
#include <mpi.h>
#include <stdio.h> 
#include <math.h>

int main(int argc, char *argv[]) 
{ 
    int remoteSize, currentRank; 
    MPI_Comm parent; 
    MPI_Status status; 
    MPI_Init(&argc, &argv); 
    MPI_Comm_get_parent(&parent); 
    if (parent == MPI_COMM_NULL) printf("===Parent intercommunicator was not created!===\n");

    MPI_Comm_remote_size(parent, &remoteSize); 
    MPI_Comm_size(MPI_COMM_WORLD, &remoteSize); 
    MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);
    
    MPI_Barrier(MPI_COMM_WORLD);

    int vectorSize;
    MPI_Bcast(&vectorSize, 1, MPI_INT, 0, parent);
    //printf("Process %i received vector size = %i\n", currentRank, vectorSize);

    int* vector = new int[vectorSize];
    MPI_Recv(vector, vectorSize, MPI_INT, 0, 0, parent, &status);

    double vectorNorm = 0;
    for(int i = 0; i < vectorSize; i++)
    {
        printf("vector[%i, %i] = %i ", currentRank, i, vector[i]);

        vectorNorm += vector[i] * vector[i];
    }
    printf("\n");
    
    vectorNorm = sqrt(vectorNorm);

    MPI_Send(&vectorNorm, 1, MPI_DOUBLE, 0, 0, parent);

    MPI_Comm_disconnect(&parent);
    MPI_Finalize(); 
    return 0; 
} 
