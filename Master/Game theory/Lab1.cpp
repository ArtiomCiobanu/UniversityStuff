//mpiCC -o Lab1.exe Lab1.cpp
//mpirun -host compute-0-1:4 Lab1.exe 8
//Добавьте 1 вторым аргументом для фиксированной матрицы: Lab1.exe 8 1
//Добавьте 0 вторым аргументом для случайной матрицы: Lab1.exe 8 0
//compute-0-1:4 - запускем на 4 процессах
//8 - размер матрицы

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <time.h>

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
	int numtask, myrank, root = 0;
	int matrixSize = 0;
	int usePresetMatrix = 0;
	double *Matr_Init = NULL;
	double *Matr_Cols = NULL;
	double *localRows = NULL;
	double *localCols = NULL;
	double *localRowMin = NULL;
	double *localColMax = NULL;
	double *allRowMin = NULL;
	double *allColMax = NULL;
	int *maximinRows = NULL;
	int *minimaxColumns = NULL;
	int *sendCounts = NULL;
	int *sendDispls = NULL;
	int *resultCounts = NULL;
	int *resultDispls = NULL;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtask);

	if (myrank == root)
	{
		if (argc > 1)
		{
			char *end = NULL;
			long requestedSize = strtol(argv[1], &end, 10);
			if (end == argv[1] || *end != '\0' || requestedSize < 1 ||
				requestedSize > INT_MAX || requestedSize > INT_MAX / requestedSize)
			{
				fprintf(stderr, "Размер матрицы должен быть положительным числом, квадрат которого не превышает INT_MAX.\n");
				MPI_Abort(MPI_COMM_WORLD, 1);
			}
			matrixSize = (int)requestedSize;
		}
		else
		{
			matrixSize = numtask;
		}

		if (argc > 2)
		{
			char *end = NULL;
			long requestedMode = strtol(argv[2], &end, 10);
			if (end == argv[2] || *end != '\0' || (requestedMode != 0 && requestedMode != 1))
			{
				fprintf(stderr, "Второй аргумент должен быть 0 (случайная) или 1 (фиксированная матрица).\n");
				MPI_Abort(MPI_COMM_WORLD, 1);
			}
			usePresetMatrix = (int)requestedMode;
		}
		if (usePresetMatrix)
			matrixSize = 3;
	}

	MPI_Bcast(&matrixSize, 1, MPI_INT, root, MPI_COMM_WORLD);
	MPI_Bcast(&usePresetMatrix, 1, MPI_INT, root, MPI_COMM_WORLD);

	if (myrank == root)
	{
		printf("\n===== Программа '%s' =====\n", argv[0]);
		printf("Numarul de procese este: %d\n", numtask);
		printf("Matricea este: %d x %d\n", matrixSize, matrixSize);
		printf("Режим матрицы: %s\n", usePresetMatrix ? "фиксированная" : "случайная");
	}

	sendCounts = (int *)malloc(numtask * sizeof(int));
	sendDispls = (int *)malloc(numtask * sizeof(int));
	resultCounts = (int *)malloc(numtask * sizeof(int));
	resultDispls = (int *)malloc(numtask * sizeof(int));
	if (!sendCounts || !sendDispls || !resultCounts || !resultDispls)
	{
		fprintf(stderr, "Процесс %d: не удалось выделить память для параметров распределения.\n", myrank);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	int rowStart = 0;
	for (int rank = 0; rank < numtask; ++rank)
	{
		int rowsForRank = matrixSize / numtask + (rank < matrixSize % numtask);
		sendCounts[rank] = rowsForRank * matrixSize;
		sendDispls[rank] = rowStart * matrixSize;
		resultCounts[rank] = rowsForRank;
		resultDispls[rank] = rowStart;
		rowStart += rowsForRank;
	}

	int localRowCount = resultCounts[myrank];
	int localValueCount = sendCounts[myrank];
	localRows = (double *)malloc((localValueCount > 0 ? localValueCount : 1) * sizeof(double));
	localCols = (double *)malloc((localValueCount > 0 ? localValueCount : 1) * sizeof(double));
	localRowMin = (double *)malloc((localRowCount > 0 ? localRowCount : 1) * sizeof(double));
	localColMax = (double *)malloc((localRowCount > 0 ? localRowCount : 1) * sizeof(double));
	if (!localRows || !localCols || !localRowMin || !localColMax)
	{
		fprintf(stderr, "Процесс %d: не удалось выделить память для локальных данных.\n", myrank);
		MPI_Abort(MPI_COMM_WORLD, 1);
	}

	if (myrank == root)
	{
		Matr_Init = (double *)malloc((size_t)matrixSize * matrixSize * sizeof(double));
		Matr_Cols = (double *)malloc((size_t)matrixSize * matrixSize * sizeof(double));
		allRowMin = (double *)malloc(matrixSize * sizeof(double));
		allColMax = (double *)malloc(matrixSize * sizeof(double));
		maximinRows = (int *)malloc(matrixSize * sizeof(int));
		minimaxColumns = (int *)malloc(matrixSize * sizeof(int));
		if (!Matr_Init || !Matr_Cols || !allRowMin || !allColMax || !maximinRows || !minimaxColumns)
		{
			fprintf(stderr, "Не удалось выделить память для матрицы.\n");
			MPI_Abort(MPI_COMM_WORLD, 1);
		}

		if (usePresetMatrix)
		{
			const double presetMatrix[9] = {
				4.0, 2.0, 1.0,
				3.0, 5.0, 1.0,
				2.0, 4.0, 0.0
			};
			memcpy(Matr_Init, presetMatrix, sizeof(presetMatrix));
		}
		else
		{
			srand((unsigned int)time(NULL));
			for (int i = 0; i < matrixSize * matrixSize; i++)
			{
				Matr_Init[i] = rand() / 1000000000.0;
			}
		}

		printf("\n===== Вывод изначальных значений =====\n");
		for (int i = 0; i < matrixSize; i++)
		{
			printf("\n");
			for (int j = 0; j < matrixSize; j++)
			{
				printf("A[%d,%d]=%5.2f ", i, j, Matr_Init[i * matrixSize + j]);
			}
		}

		printf("\n\n");
		memcpy(Matr_Cols, Matr_Init, (size_t)matrixSize * matrixSize * sizeof(double));
		transpose_inplace(Matr_Cols, matrixSize);
	}

	MPI_Scatterv(Matr_Init, sendCounts, sendDispls, MPI_DOUBLE,
		localRows, localValueCount, MPI_DOUBLE, root, MPI_COMM_WORLD);

	printf("Процесс %d получил %d строк: ", myrank, localRowCount);
	for (int i = 0; i < localValueCount; ++i)
	{
		printf("%5.2f ", localRows[i]);
	}

	MPI_Scatterv(Matr_Cols, sendCounts, sendDispls, MPI_DOUBLE,
		localCols, localValueCount, MPI_DOUBLE, root, MPI_COMM_WORLD);

	printf("\nПроцесс %d получил %d столбцов: ", myrank, localRowCount);
	for (int i = 0; i < localValueCount; ++i)
	{
		printf("%5.2f ", localCols[i]);
	}

	printf("\n");

	for (int row = 0; row < localRowCount; ++row)
	{
		localRowMin[row] = DBL_MAX;
		localColMax[row] = -DBL_MAX;
		for (int column = 0; column < matrixSize; ++column)
		{
			double rowValue = localRows[row * matrixSize + column];
			double colValue = localCols[row * matrixSize + column];
			if (rowValue < localRowMin[row])
				localRowMin[row] = rowValue;
			if (colValue > localColMax[row])
				localColMax[row] = colValue;
		}
	}

	MPI_Gatherv(localRowMin, localRowCount, MPI_DOUBLE, allRowMin,
		resultCounts, resultDispls, MPI_DOUBLE, root, MPI_COMM_WORLD);
	MPI_Gatherv(localColMax, localRowCount, MPI_DOUBLE, allColMax,
		resultCounts, resultDispls, MPI_DOUBLE, root, MPI_COMM_WORLD);

	if (myrank == root)
	{
		printf("\n===== Результаты =====\n");

		printf("\nМинимальные значения по строкам:\n");
		double lowerValue = -DBL_MAX; // нижняя цена игры = max_i min_j a_ij
		for (int i = 0; i < matrixSize; ++i)
		{
			printf("  min(linia %d) = %5.2f\n", i, allRowMin[i]);
			if (allRowMin[i] > lowerValue)
			{
				lowerValue = allRowMin[i];
			}
		}

		printf("\nМаксимальные значения по столбцам:\n");
		double upperValue = DBL_MAX; // верхняя цена игры = min_j max_i a_ij
		for (int i = 0; i < matrixSize; ++i)
		{
			printf("  max(столбца %d) = %5.2f\n", i, allColMax[i]);
			if (allColMax[i] < upperValue)
			{
				upperValue = allColMax[i];
			}
		}

		printf("\nmax min по строкам = %5.2f\n", lowerValue);
		printf("min max по столбцам = %5.2f\n", upperValue);

		int maximinRowCount = 0;
		int minimaxColumnCount = 0;
		for (int i = 0; i < matrixSize; ++i)
		{
			if (allRowMin[i] == lowerValue)
				maximinRows[maximinRowCount++] = i;
			if (allColMax[i] == upperValue)
				minimaxColumns[minimaxColumnCount++] = i;
		}

		printf("\nПары (строка, столбец):\n");
		if (lowerValue == upperValue)
		{
			int firstPair = 1;
			for (int row = 0; row < maximinRowCount; ++row)
			{
				for (int column = 0; column < minimaxColumnCount; ++column)
				{
					printf("%s(%d,%d)", firstPair ? "" : ", ",
						maximinRows[row] + 1, minimaxColumns[column] + 1);
					firstPair = 0;
				}
			}
			printf("\nЦена игры = %5.2f\n", lowerValue);
		}
		else
		{
			printf("Седловой точки в чистых стратегиях нет.\n");
		}

	}

	free(localRows);
	free(localCols);
	free(localRowMin);
	free(localColMax);
	free(sendCounts);
	free(sendDispls);
	free(resultCounts);
	free(resultDispls);
	if (myrank == root)
	{
		free(Matr_Init);
		free(Matr_Cols);
		free(allRowMin);
		free(allColMax);
		free(maximinRows);
		free(minimaxColumns);
		printf("\nКонец\n");
	}

	MPI_Finalize();
	return 0;
}