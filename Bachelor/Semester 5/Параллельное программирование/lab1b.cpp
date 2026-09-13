#include <mpi.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

void PrintMatrix(int *matrix, int rowAmount, int columnAmount)
{
    for(int i = 0; i < rowAmount; i++)
    {
        for(int j = 0; j < columnAmount; j++)
        {
            printf("M[%i, %i] = %i; ", i, j, matrix[i * columnAmount + j]); 
        }

        cout << endl;
    }
}

int *GetTransposedMatrix(int *originalMatrix, int rowAmount, int columnAmount)
{
    cout << endl;

    int *transposedMatrix = (int *) malloc(rowAmount * columnAmount * sizeof(int));

    for(int i = 0; i < columnAmount; i++)
    {
        for(int j = 0; j < rowAmount; j++)
        {
            transposedMatrix[i * rowAmount + j] = originalMatrix[j * columnAmount + i];
        }
    }

    delete [] originalMatrix;

    return transposedMatrix;
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

    receiveCount = elementsToSend[currentRank];
    currentProcessData = new int[receiveCount];

    printf("\nThe process with rank %i has received %i elements", 
        currentRank, receiveCount);
    printf("\nThe received column(s):\n");

    for (int i = 0; i < receiveCount; i++)
    {
        currentProcessData[i] = i + currentRank; //

        cout << "M[" << i << "] = " << currentProcessData[i] << "; ";

        if(i > 0 && (i + 1) % columnAmount == 0)
        {
            cout << endl;
        }
    }

    sleep(2 * currentRank);
    MPI_Barrier(MPI_COMM_WORLD);

    int sendLength = receiveCount;
    int *finalMatrix = (int *) malloc(rowAmount * columnAmount * sizeof(int));

    MPI_Gatherv(currentProcessData, 
        sendLength, 
        MPI_INT, 
        finalMatrix, 
        elementsToSend,  
        displacements,
        MPI_INT,
        rootRank, 
        MPI_COMM_WORLD);
        
    MPI_Barrier(MPI_COMM_WORLD);

    if(currentRank == rootRank)
    {
        cout << "The root rank receives the columns:" << endl;

        PrintMatrix(finalMatrix, rowAmount, columnAmount);

        finalMatrix = GetTransposedMatrix(finalMatrix, rowAmount, columnAmount);

        int c = rowAmount;
        rowAmount = columnAmount;
        columnAmount = c;

        // printf("Transposed matrix:\n");
        // PrintMatrix(finalMatrix, rowAmount, columnAmount);
        // printf("\n");

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
        }

        for (int i = 0; i < rowAmount; i++)
        {
            for(int j = 1; j < columnAmount; j++)
            {                
                int currentIndex = i * columnAmount + j;

                int currentValue = finalMatrix[currentIndex];
                if(indexValuePairs[i].value == currentValue)
                {
                    printf("M[%i,%i] = %i is the max element\n", 
                        i, j, currentValue);
                }
            }
        }
    }

    MPI_Finalize();

    return 0;
}
