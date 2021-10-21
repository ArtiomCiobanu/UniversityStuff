#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

using namespace std;

/*i- linia j- coloana
 Paralelizarea la nivel de date se face astfel:
 Procesul cu rankul rootRank initializeaza cu valori matricele A si B
*/
// void invertMatrix(double *m, int mRows, int mCols, double *rez)
// {
//     for (int i = 0; i < mRows; ++i)
//         for (int j = 0; j < mCols; ++j)
//             rez[j * mRows + i] = m[i * mCols + j];
// }

//static int maxSum;

void SendMaxElement(int *, int *, int *, MPI_Datatype *);

void SendMaxElement(int *invec, int *inoutvec, int *len, MPI_Datatype *dtype)
{
    for(int i = 0; i < *len; i++)
    {
        if(inoutvec[i] < invec[i])
        {
            inoutvec[i] = invec[i];
        }
    }
}

int main(int argc, char *argv[])
{
    int size, currentRank, rootRank = 0;
    MPI_Status status;

    MPI_Op op;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rowAmount, columnAmount;

    if (currentRank == rootRank)
    {
        printf("\n===== Results '%s' =====\n", argv[0]);

        cout << "Enter line amount: ";
        cin >> rowAmount;
        cout << "Enter column amount: ";
        cin >> columnAmount;

        
       
        int rowAmountPerProcess = rowAmount / size;
        int rowsLeft = rowAmount % size;

        int* rowAmountForEachProcess = new int[size]; //(int *) malloc(size * sizeof(int));
        for(int i = 0; i < rowsLeft; i++)
        {
            rowAmountForEachProcess[i] = rowAmountPerProcess + 1;
        }

        for(int i = rowsLeft; i < size; i++)
        {
            rowAmountForEachProcess[i] = rowAmountPerProcess;
        }

        for(int i = 0; i < size; i++)
        {
            MPI_Send(
                &(rowAmountForEachProcess[i]),
                1,
                MPI_INT,
                i,
                0,
                MPI_COMM_WORLD);
        }
    }

    int rowAmountForCurrentProcess;
    MPI_Recv(
        &rowAmountForCurrentProcess,
        1,
        MPI_INT,
        0,
        0,
        MPI_COMM_WORLD,
        &status
    );


    MPI_Barrier(MPI_COMM_WORLD);

    printf("\nProcess %i initializes %i rows. \n", currentRank, rowAmountForCurrentProcess);

    // int Row[size];
    // for (int i = 0; i < size; i++)
    // {
    //     Row[i] = 1;//i + currentRank;
    //     printf("M[%i,%i] = %i;  ", currentRank, i, Row[i]);
    // }
    // printf("\n");

    // int maxIndex = 0;
    // int rowMax = Row[maxIndex];
    // for(int i = 1; i < size; i++)
    // {
    //     if(Row[i] > rowMax)
    //     {
    //         rowMax = Row[i];
    //         maxIndex = i;
    //     }
    // }
    // printf("The max element is: M[%i,%i] = %i\n", currentRank, maxIndex, Row[maxIndex]);

    // sleep(2 * currentRank);

    // int max;
    // MPI_Op_create((MPI_User_function *) SendMaxElement, 1, &op);
    // MPI_Reduce(&rowMax, &max, 1, MPI_INT, op, 0, MPI_COMM_WORLD);

    // MPI_Op_free(&op);

    // MPI_Bcast(&max, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // if(Row[maxIndex] == max)
    // {
    //     printf("\nMax element %i is in %i process\n", max, currentRank);
    // }
   
    MPI_Finalize();
    return 0;
}

