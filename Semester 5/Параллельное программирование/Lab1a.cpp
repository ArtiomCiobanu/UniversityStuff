#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

void PrintMatrix(double *matrix, int rowAmount, int columnAmount)
{
    for(int i = 0; i < rowAmount; i++)
    {
        for(int j = 0; j < columnAmount; j++)
        {
            printf("M[%i, %i] = %4.4f; ", i, j, matrix[i * columnAmount + j]); 
        }

        cout << endl;
    }
}

struct Index
{
    double val;
    int rank;
};

Index *maxIndices;    
int rowAmount, columnAmount, sendingRowAmount;

void MaxElements(int *invec, int *inoutvec, int *len, MPI_Datatype *dtype)
{
    printf("\n");
    for (int i = 0; i < *len; i++ )
    {
        int currentRow = i / columnAmount;
        printf("invec[%i] = %i; inoutvec[%i] = %i; row: %i\n", i, invec[i], i, inoutvec[i], currentRow);
    } 
}

int main(int argc, char *argv[])
{
    int size, receiveCount;
    int currentRank, rootRank = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    maxIndices = new Index[size];

    int displacements[size];
    int elementsToSend[size];

    int *currentProcessData;

    if (currentRank == rootRank)
    {
        printf("\n========== Program '%s' ==========\n", argv[0]);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(currentRank == rootRank)
    {
        cout << "Enter line amount: ";
        cin >> rowAmount;
        cout << "Enter column amount: ";
        cin >> columnAmount;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(&rowAmount, 1, MPI_INT, rootRank, MPI_COMM_WORLD);
    MPI_Bcast(&columnAmount, 1, MPI_INT, rootRank, MPI_COMM_WORLD);

    int rowsPerProcess = rowAmount / size;
    int rowsLeft = rowAmount % size;

    int displacement = 0;
    for (int i = 0; i < size; i++)
    {
        displacements[i] = displacement;

        if (i < rowsLeft)
            elementsToSend[i] = (rowsPerProcess + 1) * columnAmount;
        else
            elementsToSend[i] = rowsPerProcess * columnAmount;
        
        displacement += elementsToSend[i];
    }

    
    int maxElementsToSend = elementsToSend[0];
    for(int i = 1; i < size; i++)
    {
        if(maxElementsToSend < elementsToSend[i])
        {
            maxElementsToSend = elementsToSend[i];
        }
    }

    receiveCount = elementsToSend[currentRank];
    int length = maxElementsToSend;
    sendingRowAmount = maxElementsToSend;
    currentProcessData = new int[length];

    for (int i = 0; i < receiveCount; i++)
    {
        currentProcessData[i] = i + currentRank;
    }

    for(int i = receiveCount; i < length; i++)
    {
        currentProcessData[i] = -1;
    }

    printf("\nThe process with rank %i has initialized %i elements", 
        currentRank, receiveCount);
    printf("\nThe received column(s):\n");

    for(int i = 0; i < length; i++)
    {
        cout << "M[" << i << "] = " << currentProcessData[i] << "; ";

        if(i > 0 && (i + 1) % columnAmount == 0)
        {
            cout << endl;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    //int sendLength = receiveCount;
    //double *finalMatrix = (double *) malloc(rowAmount * columnAmount * sizeof(double));

    MPI_Op op;
    MPI_Op_create((MPI_User_function *)MaxElements, 1, &op);

    int result;
    MPI_Reduce(
        currentProcessData,
        &result,
        length,
        MPI_INT,
        op,
        rootRank,
        MPI_COMM_WORLD
    );


   
    MPI_Finalize();

    return 0;
}
