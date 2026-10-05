// mpiCC -o Lab1_zones.exe Lab1_zones.cpp
// mpirun -host compute-0-1:4 Lab1_zones.exe 8 0
// Добавьте 1 вторым аргументом для фиксированной матрицы: Lab1_zones.exe 8 1
// Добавьте 0 вторым аргументом для случайной матрицы: Lab1_zones.exe 8 0
// compute-0-1:4 - запускаем на 4 процессах (2x2 сетка)
// 8 - размер матрицы

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <time.h>
#include <math.h>

typedef struct
{
    int rank;
    int row_start;
    int row_end;
    int col_start;
    int col_end;
    int rows;
    int cols;
} Zone;

//Создает информацию о зонах для 2D разбиения матрицы
Zone *create_zones(int global_rows, int global_cols,
                   int proc_grid_rows, int proc_grid_cols,
                   int total_procs)
{
    Zone *zones = (Zone *)malloc(total_procs * sizeof(Zone));
    if (!zones)
    {
        fprintf(stderr, "Ошибка выделения памяти для зон\n");
        return NULL;
    }

    int base_rows = global_rows / proc_grid_rows;
    int extra_rows = global_rows % proc_grid_rows;

    int base_cols = global_cols / proc_grid_cols;
    int extra_cols = global_cols % proc_grid_cols;

    int rank = 0;

    for (int i = 0; i < proc_grid_rows; i++)
    {
        for (int j = 0; j < proc_grid_cols; j++)
        {
            Zone *z = &zones[rank];

            // Вычисляем границы строк
            z->row_start = i * base_rows + (i < extra_rows ? i : extra_rows);
            z->rows = base_rows + (i < extra_rows ? 1 : 0);
            z->row_end = z->row_start + z->rows;

            // Вычисляем границы столбцов
            z->col_start = j * base_cols + (j < extra_cols ? j : extra_cols);
            z->cols = base_cols + (j < extra_cols ? 1 : 0);
            z->col_end = z->col_start + z->cols;

            z->rank = rank;

            rank++;
        }
    }

    return zones;
}

//Выделяет подматрицу (зону) из глобальной матрицы
double *extract_zone(double *global_matrix, Zone *zone, int global_cols)
{
    double *zone_matrix = (double *)malloc(zone->rows * zone->cols * sizeof(double));
    if (!zone_matrix)
    {
        fprintf(stderr, "Ошибка выделения памяти для зоны\n");
        return NULL;
    }

    for (int i = 0; i < zone->rows; i++)
    {
        for (int j = 0; j < zone->cols; j++)
        {
            int global_row = zone->row_start + i;
            int global_col = zone->col_start + j;
            zone_matrix[i * zone->cols + j] =
                global_matrix[global_row * global_cols + global_col];
        }
    }

    return zone_matrix;
}

int main(int argc, char *argv[])
{
    int numtask, myrank, root = 0;
    int matrixSize = 0;
    int usePresetMatrix = 0;
    double *Matr_Init = NULL;
    double *localZone = NULL;
    double *localRowMin = NULL;
    double *localColMax = NULL;
    double *allRowMin = NULL;
    double *allColMax = NULL;
    int *maximinRows = NULL;
    int *minimaxColumns = NULL;

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

    // ========== ОПРЕДЕЛЕНИЕ СЕТКИ ПРОЦЕССОВ ==========

    int proc_grid_rows = (int)sqrt((double)numtask);
    int proc_grid_cols = numtask / proc_grid_rows;

    // Проверка, что количество процессов подходит для 2D сетки
    if (proc_grid_rows * proc_grid_cols != numtask)
    {
        if (myrank == root)
        {
            fprintf(stderr, "Ошибка: количество процессов (%d) не подходит для 2D сетки.\n", numtask);
            fprintf(stderr, "Используйте количество процессов: 1, 4, 9, 16, 25, ...\n");
        }
        MPI_Finalize();
        return 1;
    }

    if (myrank == root)
    {
        printf("\n===== Программа '%s' (зонированное разбиение) =====\n", argv[0]);
        printf("Количество процессов: %d (%dx%d сетка)\n", numtask, proc_grid_rows, proc_grid_cols);
        printf("Матрица: %d x %d\n", matrixSize, matrixSize);
        printf("Режим матрицы: %s\n", usePresetMatrix ? "фиксированная" : "случайная");
    }

    // ========== СОЗДАНИЕ ЗОН ==========

    Zone *zones = create_zones(matrixSize, matrixSize,
                               proc_grid_rows, proc_grid_cols, numtask);
    if (!zones)
    {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    Zone my_zone = zones[myrank];
    int localValueCount = my_zone.rows * my_zone.cols;

    localZone = (double *)malloc((localValueCount > 0 ? localValueCount : 1) * sizeof(double));
    localRowMin = (double *)malloc((my_zone.rows > 0 ? my_zone.rows : 1) * sizeof(double));
    localColMax = (double *)malloc((my_zone.cols > 0 ? my_zone.cols : 1) * sizeof(double));

    if (!localZone || !localRowMin || !localColMax)
    {
        fprintf(stderr, "Процесс %d: ошибка выделения памяти для локальных данных.\n", myrank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    if (myrank == root)
    {
        Matr_Init = (double *)malloc((size_t)matrixSize * matrixSize * sizeof(double));
        allRowMin = (double *)malloc(matrixSize * sizeof(double));
        allColMax = (double *)malloc(matrixSize * sizeof(double));
        maximinRows = (int *)malloc(matrixSize * sizeof(int));
        minimaxColumns = (int *)malloc(matrixSize * sizeof(int));

        if (!Matr_Init || !allRowMin || !allColMax || !maximinRows || !minimaxColumns)
        {
            fprintf(stderr, "Ошибка выделения памяти для матрицы.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        if (usePresetMatrix)
        {
            const double presetMatrix[9] = {
                4.0, 2.0, 1.0,
                3.0, 5.0, 1.0,
                2.0, 4.0, 0.0};
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

        printf("\n===== Исходная матрица =====\n");
        for (int i = 0; i < matrixSize; i++)
        {
            printf("\n");
            for (int j = 0; j < matrixSize; j++)
            {
                printf("A[%d,%d]=%5.2f ", i, j, Matr_Init[i * matrixSize + j]);
            }
        }
        printf("\n\n");
    }

    if (myrank == root)
    {
        // Отправляем каждому процессу его зону
        for (int rank = 0; rank < numtask; rank++)
        {
            Zone *z = &zones[rank];
            double *zone_data = extract_zone(Matr_Init, z, matrixSize);

            if (rank == 0)
            {
                // Процесс 0 копирует свою зону
                memcpy(localZone, zone_data, z->rows * z->cols * sizeof(double));
                free(zone_data);
            }
            else
            {
                // Отправляем другим процессам
                MPI_Send(zone_data, z->rows * z->cols, MPI_DOUBLE, rank, 0, MPI_COMM_WORLD);
                free(zone_data);
            }
        }
    }
    else
    {
        // Получаем свою зону
        MPI_Status status;
        MPI_Recv(localZone, localValueCount, MPI_DOUBLE, root, 0, MPI_COMM_WORLD, &status);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    printf("\nПроцесс %d получил зону [строки %d-%d, столбцы %d-%d]:\n",
           myrank, my_zone.row_start, my_zone.row_end - 1,
           my_zone.col_start, my_zone.col_end - 1);

    for (int i = 0; i < my_zone.rows; i++)
    {
        for (int j = 0; j < my_zone.cols; j++)
        {
            printf("%5.2f ", localZone[i * my_zone.cols + j]);
        }
        printf("\n");
    }

    // Поиск минимумов по строкам в локальной зоне
    for (int i = 0; i < my_zone.rows; i++)
    {
        localRowMin[i] = DBL_MAX;
        for (int j = 0; j < my_zone.cols; j++)
        {
            double value = localZone[i * my_zone.cols + j];
            if (value < localRowMin[i])
                localRowMin[i] = value;
        }
    }

    // Поиск максимумов по столбцам в локальной зоне
    for (int j = 0; j < my_zone.cols; j++)
    {
        localColMax[j] = -DBL_MAX;
        for (int i = 0; i < my_zone.rows; i++)
        {
            double value = localZone[i * my_zone.cols + j];
            if (value > localColMax[j])
                localColMax[j] = value;
        }
    }

    // Собираем минимумы строк (каждый процесс имеет полные строки, поэтому используем Gather)
    if (myrank == root)
    {
        for (int i = 0; i < my_zone.rows; i++)
        {
            allRowMin[my_zone.row_start + i] = localRowMin[i];
        }

        // Получаем минимумы строк от других процессов
        for (int rank = 1; rank < numtask; rank++)
        {
            Zone *z = &zones[rank];
            double *recv_row_min = (double *)malloc(z->rows * sizeof(double));
            MPI_Recv(recv_row_min, z->rows, MPI_DOUBLE, rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < z->rows; i++)
            {
                allRowMin[z->row_start + i] = recv_row_min[i];
            }
            free(recv_row_min);
        }
    }
    else
    {
        // Другие процессы отправляют минимумы строк процессу 0
        MPI_Send(localRowMin, my_zone.rows, MPI_DOUBLE, root, 1, MPI_COMM_WORLD);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // Синхронизируем максимумы столбцов (каждый столбец может быть в нескольких процессах)
    // Используем Allreduce с MPI_MAX для правильного вычисления глобальных максимумов
    double *global_col_max = (double *)malloc(matrixSize * sizeof(double));
    double *local_col_max_full = (double *)malloc(matrixSize * sizeof(double));

    // Инициализируем локальный массив -DBL_MAX
    for (int j = 0; j < matrixSize; j++)
    {
        local_col_max_full[j] = -DBL_MAX;
    }

    // Заполняем позиции для своих столбцов
    for (int j = 0; j < my_zone.cols; j++)
    {
        local_col_max_full[my_zone.col_start + j] = localColMax[j];
    }

    // Редуцируем со всеми процессами, беря максимум
    MPI_Allreduce(local_col_max_full, global_col_max, matrixSize, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    // На процессе 0 копируем результаты
    if (myrank == root)
    {
        memcpy(allColMax, global_col_max, matrixSize * sizeof(double));
    }

    free(local_col_max_full);
    free(global_col_max);

    MPI_Barrier(MPI_COMM_WORLD);

    if (myrank == root)
    {
        printf("\n===== Результаты анализа =====\n");

        printf("\nМинимальные значения по строкам:\n");
        double lowerValue = -DBL_MAX; // max_i min_j a_ij
        for (int i = 0; i < matrixSize; i++)
        {
            printf("  min(строка %d) = %5.2f\n", i, allRowMin[i]);
            if (allRowMin[i] > lowerValue)
            {
                lowerValue = allRowMin[i];
            }
        }

        printf("\nМаксимальные значения по столбцам:\n");
        double upperValue = DBL_MAX; // min_j max_i a_ij
        for (int j = 0; j < matrixSize; j++)
        {
            printf("  max(столбец %d) = %5.2f\n", j, allColMax[j]);
            if (allColMax[j] < upperValue)
            {
                upperValue = allColMax[j];
            }
        }

        printf("\nmax min по строкам (MAXMIN) = %5.2f\n", lowerValue);
        printf("min max по столбцам (MINMAX) = %5.2f\n", upperValue);

        int maximinRowCount = 0;
        int minimaxColumnCount = 0;

        for (int i = 0; i < matrixSize; i++)
        {
            if (allRowMin[i] == lowerValue)
            {
                maximinRows[maximinRowCount++] = i;
            }
        }

        for (int j = 0; j < matrixSize; j++)
        {
            if (allColMax[j] == upperValue)
            {
                minimaxColumns[minimaxColumnCount++] = j;
            }
        }

        printf("\n===== РАВНОВЕСНЫЕ ИСХОДЫ =====\n");
        printf("Пары (строка, столбец):\n");

        if (lowerValue == upperValue)
        {
            printf("✓ РАВНОВЕСИЕ СУЩЕСТВУЕТ\n");
            printf("Значение игры (цена) = %5.2f\n\n", lowerValue);

            int firstPair = 1;
            for (int row = 0; row < maximinRowCount; row++)
            {
                for (int col = 0; col < minimaxColumnCount; col++)
                {
                    if (!firstPair)
                        printf(", ");
                    printf("(%d,%d)", maximinRows[row] + 1, minimaxColumns[col] + 1);
                    firstPair = 0;
                }
            }
            printf("\n");

            printf("\nОптимальные стратегии:\n");
            printf("  Игрок 1 (строки): ");
            for (int row = 0; row < maximinRowCount; row++)
            {
                if (row > 0)
                    printf(", ");
                printf("%d", maximinRows[row] + 1);
            }
            printf("\n");

            printf("  Игрок 2 (столбцы): ");
            for (int col = 0; col < minimaxColumnCount; col++)
            {
                if (col > 0)
                    printf(", ");
                printf("%d", minimaxColumns[col] + 1);
            }
            printf("\n");
        }
        else
        {
            printf("✗ СЕДЛОВОЙ ТОЧКИ В ЧИСТЫХ СТРАТЕГИЯХ НЕ СУЩЕСТВУЕТ\n");
            printf("Требуется анализ смешанных стратегий.\n");
            printf("  MAXMIN = %5.2f < MINMAX = %5.2f\n", lowerValue, upperValue);
        }
    }

    free(localZone);
    free(localRowMin);
    free(localColMax);
    free(zones);

    if (myrank == root)
    {
        free(Matr_Init);
        free(allRowMin);
        free(allColMax);
        free(maximinRows);
        free(minimaxColumns);
        printf("\nКонец\n");
    }

    MPI_Finalize();
    return 0;
}
