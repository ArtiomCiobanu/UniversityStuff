#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>

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

void CountMaxSum(int *, int *, int *, MPI_Datatype *);

void CountMaxSum(int *invec, int *inoutvec, int *len, MPI_Datatype *dtype)
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
    int size;
    int currentRank, rootRank = 0;

    MPI_Op op;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (currentRank == rootRank)
    {
        printf("===== Results '%s' =====\n", argv[0]);
    }

    printf("Process %i initializes its row. \n", currentRank);

    int Row[size];
    for (int i = 0; i < size; i++)
    {
        Row[i] = i + currentRank;
        printf("M[%i,%i] = %i;  ", currentRank, i, Row[i]);
    }
    printf("\n");

    int sum = 0;
    for(int i = 0; i < size; i++)
    {
        sum += Row[i];
    }

    printf("%i rank\'s row sum: %i\n", currentRank, sum);

    sleep(2 * currentRank);

    int data = sum;
    int maxSum = 0;
    MPI_Op_create((MPI_User_function *) CountMaxSum, 1, &op);
    MPI_Reduce(&data, &maxSum, 1, MPI_INT, op, 0, MPI_COMM_WORLD);

    if(currentRank == rootRank)
    {
        printf("\nMax sum: %i\n", maxSum);
    }
    
    MPI_Op_free(&op);
    
    MPI_Finalize();
    return 0;
}

