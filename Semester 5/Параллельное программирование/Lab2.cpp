#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

using namespace std;

enum SIDE 
{
	X_FIRST, 	//side where x = 0
	X_LAST, 	//side where x = cube size
	Y_FIRST, 	//side where y = 0
	Y_LAST, 	//side where y = cube size
	Z_FIRST,	//side where z = 0
	Z_LAST		//side where z = cube size
};

const SIDE FIXED_SIDE = X_LAST; 
const int rootRank = 0; 
const int tag = 111; //Tag for MPI_Send, MPI_Recv
const int DimensionAmount = 3;
const int RELATIVE_TO_AXIS = 0; //Shows if hour hand direction is for every axis or for each side separately(0 for each, 1 - for the axis)

struct Point3D 
{
	Point3D() { }
	Point3D(int x, int y, int z)
    {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	int x;
	int y;
	int z;
};

struct Point2D 
{
	Point2D() { }
	Point2D(int x, int y) 
    {
		this->x = x;
		this->y = y;
	}

	int x;
	int y;

	bool operator==(Point2D rhs) 
    {
		return this->x == rhs.x && this->y == rhs.y;
	}
};


struct SidePoint 
{
	SidePoint() { }
	SidePoint(int rank, Point3D source, Point2D translated) 
    {
		this->rank = rank;
		this->source = source;
		this->translated = translated;
	}

	int rank;
	Point3D source; 
	Point2D translated; 
};

bool CanCreateCube(int processAmount, int *side_length) 
{
    //The amount should the cube dimension in power 3
	int curoot = round(pow(processAmount, 1.0/3.0));
	*side_length = curoot;
	return curoot * curoot * curoot == processAmount;
}

bool isPointOnSide(Point3D &point, SIDE side, int side_length) 
{
	int last_coord = side_length - 1;
	switch (side) {
		case X_FIRST:
			return point.x == 0 && (point.y == 0 || point.y == last_coord || point.z == 0 || point.z == last_coord); 
		case X_LAST:
			return point.x == last_coord && (point.y == 0 || point.y == last_coord || point.z == 0 || point.z == last_coord);
		case Y_FIRST:
			return point.y == 0 && (point.x == 0 || point.x == last_coord || point.z == 0 || point.z == last_coord);
		case Y_LAST:
			return point.y == last_coord && (point.x == 0 || point.x == last_coord || point.z == 0 || point.z == last_coord); 
		case Z_FIRST:
			return point.z == 0 && (point.x == 0 || point.x == last_coord || point.z == 0 || point.z == last_coord); 
		case Z_LAST:
			return point.z == last_coord && (point.x == 0 || point.x == last_coord || point.y == 0 || point.y == last_coord);
		default:
			return false;
	}
}

void translateTo2D(SIDE side, Point3D &point3d, Point2D *point2d) 
{
	switch (side) 
    {
		case X_FIRST:
		case X_LAST:
			//если зафиксировали грань x, то нас интересуют координаты y и z
			point2d->x = point3d.y;
			point2d->y = point3d.z;
			break;
		case Y_FIRST:
		case Y_LAST:
			//если зафиксировали грань y, то нас интересуют координаты x и z
			point2d->x = point3d.x;
			point2d->y = point3d.z;
			break;
		case Z_FIRST:
		case Z_LAST:
			//если зафиксировали грань z, то нас интересуют координаты x и y
			point2d->x = point3d.x;
			point2d->y = point3d.y;
			break;
	}
}

SidePoint *findBy2dCoord(SidePoint *sidePoints, int ranks_count, Point2D coord) 
{
	for (int i = 0; i < ranks_count; i++) 
    {
		if (sidePoints[i].translated == coord)
        {
			return &sidePoints[i];
        }
	}

	return NULL;
}


int *getRanksOnSide(SIDE side, int side_length, int *ranks_count) 
{
	int last_coord = side_length - 1;
	*ranks_count = 4 * last_coord;
	int *ranks = (int*)malloc(*ranks_count * sizeof(int));
	SidePoint *data = (SidePoint*)malloc(*ranks_count * sizeof(SidePoint));
	int current_rank = 0, i = 0;
	Point3D coord;
	Point2D coord_translated;

	for (int x = 0; x < side_length; x++) 
    {
		coord.x = x;
		for (int y = 0; y < side_length; y++) 
        {
			coord.y = y;
			for (int z = 0; z < side_length; z++, current_rank++) 
            {
				coord.z = z;
				if (!isPointOnSide(coord, side, side_length))
                {
					continue;
                }

				//if the point (x, y, z) is on the given side, changing it to 2d coordinates
				translateTo2D(side, coord, &coord_translated);
				data[i++] = SidePoint(current_rank, coord, coord_translated);
			}
		}
	}

	//Sorting in the order of hour hand
	i = 0;
	Point2D p(0,0);
	SidePoint *ordered;

	do 
	{
		ordered = findBy2dCoord(data, *ranks_count, p);
		ordered != NULL && (ranks[i++] = ordered->rank);
		p.x++;
	}
	while (p.x < last_coord);

	do 
	{
		ordered = findBy2dCoord(data, *ranks_count, p);
		ordered != NULL && (ranks[i++] = ordered->rank);
		p.y++;
	}
	while (p.y < last_coord);

	do 
	{
		ordered = findBy2dCoord(data, *ranks_count, p);
		ordered != NULL && (ranks[i++] = ordered->rank);
		p.x--;
	}
	while (p.x > 0);

	do 
	{
		ordered = findBy2dCoord(data, *ranks_count, p);
		ordered != NULL && (ranks[i++] = ordered->rank);
		p.y--;
	}
	while (p.y > 0);

	return ranks;
}

int main(int argc, char **argv) 
{
	int currentRank, size, side_length, side_rank;
	Point3D coords;

	MPI_Comm comm, side_comm, side_ring_comm;
	MPI_Group MPI_GROUP_WORLD, side_group;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	bool canCreateCube = CanCreateCube(size, &side_length);
	if (!canCreateCube) 
    {
		if (currentRank == rootRank)
        {
			printf("Can not create cube using %d processes.\n", size);
        }

		MPI_Finalize();
		exit(1);
	}

	int dimensions[DimensionAmount] = { side_length, side_length, side_length };
	int periods[DimensionAmount] = { 0, 0, 0 };
	int coordinates[DimensionAmount];
	
	MPI_Cart_create(MPI_COMM_WORLD, DimensionAmount, dimensions, periods, 0, &comm);
	
	MPI_Cart_coords(comm, currentRank, DimensionAmount, coordinates);

	coords.x = coordinates[0]; 
	coords.y = coordinates[1]; 
	coords.z = coordinates[2];

	//Getting ranks located on edges of fixed sides
	int ranks_on_side_count;
	int *ranks_on_side = getRanksOnSide(FIXED_SIDE, side_length, &ranks_on_side_count);

	MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD);
	
	MPI_Group_incl(MPI_GROUP_WORLD, ranks_on_side_count, ranks_on_side, &side_group);
	
	MPI_Comm_create(MPI_COMM_WORLD, side_group, &side_comm);
	
	MPI_Group_rank(side_group, &side_rank);

	if (side_rank != MPI_UNDEFINED) 
	{
		int side_comm_size, source, destination;
		int side_card_persiods[1] = { 1 };
		MPI_Comm_size(side_comm, &side_comm_size);
		MPI_Cart_create(side_comm, 1, &side_comm_size, side_card_persiods, 0, &side_ring_comm);

		int displ = -1;
		if (!RELATIVE_TO_AXIS && (FIXED_SIDE == X_FIRST || FIXED_SIDE == Y_FIRST || FIXED_SIDE == Z_FIRST))
		{
			displ = 1;
		}

		//sending neighbours
		MPI_Cart_shift(side_ring_comm, 1, displ, &source, &destination);

		//sending data by circle
		int bufferSize = DimensionAmount + 2;
		int sendbuf[bufferSize] = { currentRank, side_rank, coords.x, coords.y, coords.z };
		int recvbuf[bufferSize]; 
		MPI_Send(sendbuf, bufferSize, MPI_INT, destination, tag, side_ring_comm);
		MPI_Recv(recvbuf, bufferSize, MPI_INT, source, tag, side_ring_comm, MPI_STATUS_IGNORE);

		printf("Process %3d (%d,%d,%d) -> %3d (%d,%d,%d)\n", sendbuf[0], sendbuf[2], sendbuf[3], sendbuf[4], recvbuf[0], recvbuf[2], recvbuf[3], recvbuf[4]);
		
		MPI_Comm_free(&side_ring_comm);
	}

	// MPI_Group_free(&side_group);
	MPI_Finalize();
	return 0;
}