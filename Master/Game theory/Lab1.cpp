#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h> // +++ DBL_MAX, -DBL_MAX

static void transpose_inplace(double *m, int n)
{
	for (int i = 0; i < n; i++)
		for (int j = i + 1; j < n; j++)
		{
			double tmp = m[i * n + j];
			m[i * n + j] = m[j * n + i];
			m[j * n + i] = tmp;
		}
}

int main(int argc, char *argv[])
{
	int numtask, sendcount, reccount, source;
	double *Matr_Init = NULL; // +++ инициализация
	int i, myrank, root = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtask);

	double Rows[numtask];
	double Cols[numtask];
	sendcount = numtask;
	reccount = numtask;

	if (myrank == 0)
	{
		printf("\n=====PORNIM PROGRAMUL '%s' =====\n", argv[0]);
		printf("Numarul de procese este: %d\n", numtask);
		printf("Matricea este: %d x %d\n", numtask, numtask);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	// Procesul cu rankul root aloca spatiul si initializeaza matrice
	if (myrank == root)
	{
		Matr_Init = (double *)malloc(numtask * numtask * sizeof(double));
		for (int i = 0; i < numtask * numtask; i++)
		{
			Matr_Init[i] = rand() / 1000000000.0;
		}

		printf("\nTipar datele initiale\n");
		for (int i = 0; i < numtask; i++)
		{
			printf("\n");
			for (int j = 0; j < numtask; j++)
			{
				printf("A[%d,%d]=%5.2f ", i, j, Matr_Init[i * numtask + j]);
			}
		}

		printf("\n\n");
	}

	MPI_Barrier(MPI_COMM_WORLD);

	MPI_Scatter(Matr_Init, sendcount, MPI_DOUBLE, Rows, reccount, MPI_DOUBLE, root, MPI_COMM_WORLD);

	printf("Procesul cu rankul %d a primit aceasta linie: ", myrank);
	for (i = 0; i < numtask; ++i)
	{
		printf("Rows[%d]=%5.2f ", i, Rows[i]);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	double *Matr_Cols = NULL;
	if (myrank == root)
	{
		Matr_Cols = (double *)malloc(numtask * numtask * sizeof(double));

		// Copiam matricea pentru transponare
		memcpy(Matr_Cols, Matr_Init, numtask * numtask * sizeof(double));

		// Transponam matricea pentru a putea distribui coloanele
		transpose_inplace(Matr_Cols, numtask);
	}

	MPI_Scatter(Matr_Cols, sendcount, MPI_DOUBLE, Cols, reccount, MPI_DOUBLE, root, MPI_COMM_WORLD);

	printf("\nProcesul cu rankul %d a primit aceasta coloana: ", myrank);
	for (i = 0; i < numtask; ++i)
	{
		printf("Cols[%d]=%5.2f ", i, Cols[i]);
	}

	printf("\n");

	MPI_Barrier(MPI_COMM_WORLD);

	//Definim minimum local al liniei
	double localRowMin = DBL_MAX; 
	//definim maximul local al coloanei
	double localColMax = -DBL_MAX;

	for (i = 0; i < numtask; ++i)
	{
		if (Rows[i] < localRowMin)
		{
			localRowMin = Rows[i];
		}

		if (Cols[i] > localColMax)
		{
			localColMax = Cols[i];
		}
	}

	printf("Procesul %d: min(Rows)=%5.2f, max(Cols)=%5.2f\n", myrank, localRowMin, localColMax);

	//Trimitem rezultatele la root
	double *allRowMin;
	double *allColMax;
	if (myrank == root)
	{
		allRowMin = (double *)malloc(numtask * sizeof(double));
		allColMax = (double *)malloc(numtask * sizeof(double));
	}

	MPI_Gather(&localRowMin, 1, MPI_DOUBLE, allRowMin, 1, MPI_DOUBLE, root, MPI_COMM_WORLD);
	MPI_Gather(&localColMax, 1, MPI_DOUBLE, allColMax, 1, MPI_DOUBLE, root, MPI_COMM_WORLD);

	MPI_Barrier(MPI_COMM_WORLD);

	if (myrank == root)
	{
		printf("\n===== REZULTATE =====\n");

		printf("\nMinimele pe linii:\n");
		double lowerValue = -DBL_MAX; // нижняя цена игры = max_i min_j a_ij
		for (i = 0; i < numtask; ++i)
		{
			printf("  min(linia %d) = %5.2f\n", i, allRowMin[i]);
			if (allRowMin[i] > lowerValue)
			{
				lowerValue = allRowMin[i];
			}
		}

		printf("\nMaximele pe coloane:\n");
		double upperValue = DBL_MAX; // верхняя цена игры = min_j max_i a_ij
		for (i = 0; i < numtask; ++i)
		{
			printf("  max(coloana %d) = %5.2f\n", i, allColMax[i]);
			if (allColMax[i] < upperValue)
			{
				upperValue = allColMax[i];
			}
		}

		printf("\nmax min pe linii = %5.2f\n", lowerValue);
		printf("min max pe coloane = %5.2f\n", upperValue);

		// if (lowerValue == upperValue)
		// {
		// 	printf("Sedlovaya tochka est: tsena igry = %5.2f\n", lowerValue);
		// }
		// else
		// {
		// 	printf("Sedlovoy tochki net (igra v smeshannykh strategiyakh)\n");
		// }

		free(allRowMin);
		free(allColMax);
	}

	MPI_Barrier(MPI_COMM_WORLD);

	if (myrank == root)
	{
		free(Matr_Init);
		free(Matr_Cols);
		printf("\nFinalizare\n");
	}

	MPI_Finalize();
	return 0;
}