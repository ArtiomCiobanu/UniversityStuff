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

int main(int argc, char *argv[])
{
    int size, receiveCount;
    int currentRank, rootRank = 0;
    
    int rowAmount, columnAmount;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int displacements[size];
    int elementsToSend[size];

    double *currentProcessData;

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

    receiveCount = elementsToSend[currentRank];
    currentProcessData = new double[receiveCount];

    printf("\nThe process with rank %i has received %i elements", 
        currentRank, receiveCount);
    printf("\nThe received column(s):\n");

    for (int i = 0; i < receiveCount; i++)
    {
        currentProcessData[i] = i + currentRank;

        cout << "M[" << i << "] = " << currentProcessData[i] << "; ";

        if(i > 0 && (i + 1) % columnAmount == 0)
        {
            cout << endl;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int sendLength = receiveCount;
    double *finalMatrix = (double *) malloc(rowAmount * columnAmount * sizeof(double));

    MPI_Gatherv(currentProcessData, 
        sendLength, 
        MPI_DOUBLE, 
        finalMatrix, 
        elementsToSend,  
        displacements,
        MPI_DOUBLE,
        rootRank, 
        MPI_COMM_WORLD);

    sleep(2 * currentRank);
    MPI_Barrier(MPI_COMM_WORLD);

    if(currentRank == rootRank)
    {
        cout << "The root rank receives the columns:" << endl;

        for (int i = 0; i < rowAmount; i++)
        {
            for(int j = 0; j < columnAmount; j++)
            {
                printf("FinalMatrix[%i, %i] = %4.4f; ", i, j, finalMatrix[i * columnAmount + j]);
            }

            cout << endl;
        }

        struct
        {
            int columnIndex;
            int value;
        } indexValuePairs[rowAmount];
        
        for (int i = 0; i < rowAmount; i++)
        {
            indexValuePairs[i].columnIndex = 0;
            indexValuePairs[i].value = finalMatrix[i * columnAmount];

            for(int j = 1; j < columnAmount; j++)
            {
                int currentIndex = i * columnAmount + j;

                int currentValue = finalMatrix[currentIndex];
                if(indexValuePairs[i].value < currentValue)
                {
                    indexValuePairs[i].value = currentValue;
                    indexValuePairs[i].columnIndex = j;
                }
            }

            printf("Row %i has the max element: %i in the column with index: %i", 
                i, indexValuePairs[i].value, indexValuePairs[i].columnIndex);
            cout << endl;
        }
    }

    MPI_Finalize();

    return 0;
}
