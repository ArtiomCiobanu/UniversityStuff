// mpiCC -o Lab2_zones Lab2_zones.cpp
// mpirun -np 4 ./Lab2_zones 8 0
//
// Аргументы:
//   1-й: размер матрицы n (по умолчанию = число процессов)
//   2-й: режим матриц
//        0 - случайные матрицы A и B размера n x n
//        1 - Пример 6.2 из методички (3x3, равновесий в чистых стратегиях нет)
//        2 - Пример 6.1 из методички (6x6)
//
// Количество процессов: 1, 4, 9, 16, ... (2D сетка процессов, как в Lab1_zones)
//
// Алгоритм (без удаления доминируемых стратегий, это необязательный шаг
// методички; см. сноску 2 в п. 6.2):
//   1) Матрицы A и B делятся на 2D-зоны, каждый процесс получает свою зону
//      в обеих матрицах (одинаковые границы строк и столбцов).
//   2) Каждый процесс находит максимумы по столбцам в своей зоне матрицы A
//      и по строкам в своей зоне матрицы B. Глобальные максимумы собираются
//      через MPI_Allreduce (MPI_MAX), как в Lab1_zones.
//   3) Клетка (i, j) в зоне процесса является равновесием Нэша, если
//      a_ij == max по столбцу j матрицы A и b_ij == max по строке i матрицы B.
//      Это и есть пересечение графиков лучших ответов из методички.
//   4) Все найденные равновесия собираются на процессе 0 через MPI_Gatherv.
//   5) Процесс 0 проверяет результат последовательным перебором.
//
// Индексы в выводе - с нуля, как в методичке.

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <time.h>

// ========== СТРУКТУРЫ ==========

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

typedef struct
{
    int i;
    int j;
} Pair;

// ========== ФУНКЦИИ РАБОТЫ С ЗОНАМИ ==========

/**
 * Создает информацию о зонах для 2D разбиения матрицы
 */
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

            z->row_start = i * base_rows + (i < extra_rows ? i : extra_rows);
            z->rows = base_rows + (i < extra_rows ? 1 : 0);
            z->row_end = z->row_start + z->rows;

            z->col_start = j * base_cols + (j < extra_cols ? j : extra_cols);
            z->cols = base_cols + (j < extra_cols ? 1 : 0);
            z->col_end = z->col_start + z->cols;

            z->rank = rank;
            rank++;
        }
    }

    return zones;
}

/**
 * Выделяет зону из глобальной матрицы
 */
double *extract_zone(double *global_matrix, Zone *zone, int global_cols)
{
    size_t count = (size_t)zone->rows * zone->cols;
    double *zone_matrix = (double *)malloc((count > 0 ? count : 1) * sizeof(double));
    if (!zone_matrix)
    {
        fprintf(stderr, "Ошибка выделения памяти для подматрицы\n");
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

// ========== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ==========

static int cmp_pair(const void *a, const void *b)
{
    const Pair *p = (const Pair *)a;
    const Pair *q = (const Pair *)b;
    if (p->i != q->i)
        return p->i - q->i;
    return p->j - q->j;
}

/**
 * Заполняет матрицы A и B выбранным режимом
 */
static void fill_matrices(int mode, int n, double *A, double *B)
{
    if (mode == 1)
    {
        // Пример 6.2 из методички
        const double A3[9] = {2, 0, 1,
                              1, 2, 0,
                              0, 1, 2};
        const double B3[9] = {1, 0, 2,
                              2, 1, 0,
                              0, 2, 1};
        memcpy(A, A3, sizeof(A3));
        memcpy(B, B3, sizeof(B3));
    }
    else if (mode == 2)
    {
        // Пример 6.1 из методички
        const double A6[36] = {400, 0, 0, 0, 0, 0,
                               300, 300, 0, 0, 0, 0,
                               200, 200, 200, 0, 0, 0,
                               100, 100, 100, 100, 0, 0,
                               0, 0, 0, 0, 0, 0,
                               -100, -100, -100, -100, -100, -100};
        const double B6[36] = {0, 200, 100, 0, -100, -200,
                               0, 0, 100, 0, -100, -200,
                               0, 0, 0, 0, -100, -200,
                               0, 0, 0, 0, -100, -200,
                               0, 0, 0, 0, 0, -200,
                               0, 0, 0, 0, 0, 0};
        memcpy(A, A6, sizeof(A6));
        memcpy(B, B6, sizeof(B6));
    }
    else
    {
        srand((unsigned int)time(NULL));
        for (int k = 0; k < n * n; k++)
        {
            A[k] = rand() / 1000000000.0;
            B[k] = rand() / 1000000000.0;
        }
    }
}

/**
 * Последовательная проверка: перебор всех ситуаций.
 * isNE[i*n+j] = 1, если (i,j) - равновесие Нэша.
 * Возвращает количество равновесий.
 */
static int serial_nash(int n, const double *A, const double *B, int *isNE)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int ok = 1;
            // игрок 1 не хочет менять строку: a_ij >= a_kj для всех k
            for (int k = 0; k < n && ok; k++)
            {
                if (A[k * n + j] > A[i * n + j])
                    ok = 0;
            }
            // игрок 2 не хочет менять столбец: b_ij >= b_il для всех l
            for (int l = 0; l < n && ok; l++)
            {
                if (B[i * n + l] > B[i * n + j])
                    ok = 0;
            }
            isNE[i * n + j] = ok;
            count += ok;
        }
    }
    return count;
}

// ========== ОСНОВНАЯ ПРОГРАММА ==========

int main(int argc, char *argv[])
{
    int numtask, myrank, root = 0;
    int n = 0;
    int mode = 0;
    double *A = NULL;
    double *B = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtask);

    // ========== ПАРАМЕТРЫ ==========

    if (myrank == root)
    {
        n = numtask;

        if (argc > 1)
        {
            char *end = NULL;
            long req = strtol(argv[1], &end, 10);
            if (end == argv[1] || *end != '\0' || req < 1 || req > 2000)
            {
                fprintf(stderr, "Размер матрицы: целое число от 1 до 2000.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            n = (int)req;
        }

        if (argc > 2)
        {
            char *end = NULL;
            long req = strtol(argv[2], &end, 10);
            if (end == argv[2] || *end != '\0' || req < 0 || req > 2)
            {
                fprintf(stderr, "Режим: 0 - случайные, 1 - пример 6.2, 2 - пример 6.1.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            mode = (int)req;
        }

        if (mode == 1)
            n = 3;
        if (mode == 2)
            n = 6;
    }

    MPI_Bcast(&n, 1, MPI_INT, root, MPI_COMM_WORLD);
    MPI_Bcast(&mode, 1, MPI_INT, root, MPI_COMM_WORLD);

    // ========== СЕТКА ПРОЦЕССОВ ==========

    int proc_grid_rows = (int)sqrt((double)numtask);
    int proc_grid_cols = numtask / proc_grid_rows;

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
        printf("\n===== Программа '%s' (зонированное разбиение, n=%d) =====\n", argv[0], n);
        printf("Количество процессов: %d (%dx%d сетка)\n", numtask, proc_grid_rows, proc_grid_cols);
        printf("Режим матриц: %s\n",
               mode == 0 ? "случайные" : (mode == 1 ? "пример 6.2" : "пример 6.1"));
    }

    // ========== ЗОНЫ ==========

    Zone *zones = create_zones(n, n, proc_grid_rows, proc_grid_cols, numtask);
    if (!zones)
    {
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    Zone my_zone = zones[myrank];
    int localCount = my_zone.rows * my_zone.cols;

    double *localA = (double *)malloc((localCount > 0 ? localCount : 1) * sizeof(double));
    double *localB = (double *)malloc((localCount > 0 ? localCount : 1) * sizeof(double));
    if (!localA || !localB)
    {
        fprintf(stderr, "Процесс %d: ошибка выделения памяти.\n", myrank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // ========== ИНИЦИАЛИЗАЦИЯ НА ROOT ==========

    if (myrank == root)
    {
        A = (double *)malloc((size_t)n * n * sizeof(double));
        B = (double *)malloc((size_t)n * n * sizeof(double));
        if (!A || !B)
        {
            fprintf(stderr, "Ошибка выделения памяти для матриц.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        fill_matrices(mode, n, A, B);

        if (n <= 10)
        {
            printf("\n===== Исходные матрицы =====\n");
            for (int i = 0; i < n; i++)
            {
                printf("\n");
                for (int j = 0; j < n; j++)
                    printf("A[%d,%d]=%6.2f ", i, j, A[i * n + j]);
            }
            printf("\n");
            for (int i = 0; i < n; i++)
            {
                printf("\n");
                for (int j = 0; j < n; j++)
                    printf("B[%d,%d]=%6.2f ", i, j, B[i * n + j]);
            }
            printf("\n\n");
        }
    }

    // ========== РАСПРЕДЕЛЕНИЕ ЗОН ==========

    if (myrank == root)
    {
        for (int r = 0; r < numtask; r++)
        {
            Zone *z = &zones[r];
            int zc = z->rows * z->cols;
            double *zA = extract_zone(A, z, n);
            double *zB = extract_zone(B, z, n);

            if (r == root)
            {
                memcpy(localA, zA, zc * sizeof(double));
                memcpy(localB, zB, zc * sizeof(double));
            }
            else
            {
                MPI_Send(zA, zc, MPI_DOUBLE, r, 0, MPI_COMM_WORLD);
                MPI_Send(zB, zc, MPI_DOUBLE, r, 1, MPI_COMM_WORLD);
            }

            free(zA);
            free(zB);
        }
    }
    else
    {
        MPI_Recv(localA, localCount, MPI_DOUBLE, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(localB, localCount, MPI_DOUBLE, root, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    // ========== ПАРАЛЛЕЛЬНАЯ ЧАСТЬ (замер времени) ==========

    double t0 = MPI_Wtime();

    // Шаг 1. Максимумы по столбцам матрицы A (по всем строкам n).
    // Для столбцов, которых нет в зоне, локальное значение = -DBL_MAX.
    double *colMaxLocal = (double *)malloc(n * sizeof(double));
    double *colMaxGlobal = (double *)malloc(n * sizeof(double));
    for (int j = 0; j < n; j++)
        colMaxLocal[j] = -DBL_MAX;

    for (int j = 0; j < my_zone.cols; j++)
    {
        double m = -DBL_MAX;
        for (int i = 0; i < my_zone.rows; i++)
        {
            double v = localA[i * my_zone.cols + j];
            if (v > m)
                m = v;
        }
        colMaxLocal[my_zone.col_start + j] = m;
    }

    MPI_Allreduce(colMaxLocal, colMaxGlobal, n, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    // Шаг 2. Максимумы по строкам матрицы B (по всем столбцам n).
    double *rowMaxLocal = (double *)malloc(n * sizeof(double));
    double *rowMaxGlobal = (double *)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++)
        rowMaxLocal[i] = -DBL_MAX;

    for (int i = 0; i < my_zone.rows; i++)
    {
        double m = -DBL_MAX;
        for (int j = 0; j < my_zone.cols; j++)
        {
            double v = localB[i * my_zone.cols + j];
            if (v > m)
                m = v;
        }
        rowMaxLocal[my_zone.row_start + i] = m;
    }

    MPI_Allreduce(rowMaxLocal, rowMaxGlobal, n, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    // Шаг 3. Проверка клеток своей зоны на равновесие Нэша.
    Pair *localNE = (Pair *)malloc((localCount > 0 ? localCount : 1) * sizeof(Pair));
    int localNECount = 0;

    for (int i = 0; i < my_zone.rows; i++)
    {
        for (int j = 0; j < my_zone.cols; j++)
        {
            int gi = my_zone.row_start + i;
            int gj = my_zone.col_start + j;
            double a = localA[i * my_zone.cols + j];
            double b = localB[i * my_zone.cols + j];

            if (a == colMaxGlobal[gj] && b == rowMaxGlobal[gi])
            {
                localNE[localNECount].i = gi;
                localNE[localNECount].j = gj;
                localNECount++;
            }
        }
    }

    // Шаг 4. Сбор равновесий на процессе 0.
    int *counts = NULL;
    int *recvcounts = NULL;
    int *displs = NULL;
    Pair *allNE = NULL;
    int totalNE = 0;

    if (myrank == root)
    {
        counts = (int *)malloc(numtask * sizeof(int));
        recvcounts = (int *)malloc(numtask * sizeof(int));
        displs = (int *)malloc(numtask * sizeof(int));
    }

    MPI_Gather(&localNECount, 1, MPI_INT, counts, 1, MPI_INT, root, MPI_COMM_WORLD);

    if (myrank == root)
    {
        for (int r = 0; r < numtask; r++)
        {
            // Pair состоит из двух int, поэтому счет и смещения в int-ах
            recvcounts[r] = 2 * counts[r];
            displs[r] = 2 * totalNE;
            totalNE += counts[r];
        }
        allNE = (Pair *)malloc((totalNE > 0 ? totalNE : 1) * sizeof(Pair));
    }

    MPI_Gatherv(localNE, 2 * localNECount, MPI_INT,
                (int *)allNE, recvcounts, displs, MPI_INT,
                root, MPI_COMM_WORLD);

    double elapsed = MPI_Wtime() - t0;
    double maxElapsed = 0.0;
    MPI_Reduce(&elapsed, &maxElapsed, 1, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);

    // ========== ВЫВОД ЗОН (по очереди) ==========

    for (int r = 0; r < numtask; r++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (myrank == r && n <= 10)
        {
            printf("Процесс %d: строки %d-%d, столбцы %d-%d, найдено равновесий в зоне: %d\n",
                   myrank,
                   my_zone.row_start, my_zone.row_end - 1,
                   my_zone.col_start, my_zone.col_end - 1,
                   localNECount);
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    // ========== РЕЗУЛЬТАТЫ (процесс 0) ==========

    if (myrank == root)
    {
        qsort(allNE, totalNE, sizeof(Pair), cmp_pair);

        printf("\n===== Равновесия Нэша в чистых стратегиях (параллельно) =====\n");
        if (totalNE == 0)
        {
            printf("Равновесий в чистых стратегиях нет.\n");
        }
        else
        {
            printf("Найдено равновесий: %d\n", totalNE);
            for (int k = 0; k < totalNE; k++)
            {
                int i = allNE[k].i;
                int j = allNE[k].j;
                printf("(%d,%d)  выигрыш 1-го: %6.2f,  выигрыш 2-го: %6.2f\n",
                       i, j, A[i * n + j], B[i * n + j]);
            }
        }

        // Проверка последовательным перебором
        int *isNE = (int *)malloc((size_t)n * n * sizeof(int));
        int serialCount = serial_nash(n, A, B, isNE);

        int *parFlag = (int *)calloc((size_t)n * n, sizeof(int));
        for (int k = 0; k < totalNE; k++)
            parFlag[allNE[k].i * n + allNE[k].j] = 1;

        int mismatch = 0;
        for (int k = 0; k < n * n; k++)
            if (parFlag[k] != isNE[k])
                mismatch = 1;

        printf("\nПроверка перебором: найдено %d равновесий - %s\n",
               serialCount,
               (!mismatch && serialCount == totalNE) ? "СОВПАДАЕТ" : "НЕ СОВПАДАЕТ");

        printf("Время параллельной части (max по процессам): %.6f с\n", maxElapsed);

        free(isNE);
        free(parFlag);
    }

    // ========== ОСВОБОЖДЕНИЕ ПАМЯТИ ==========

    free(localA);
    free(localB);
    free(localNE);
    free(colMaxLocal);
    free(colMaxGlobal);
    free(rowMaxLocal);
    free(rowMaxGlobal);
    free(zones);

    if (myrank == root)
    {
        free(A);
        free(B);
        free(counts);
        free(recvcounts);
        free(displs);
        free(allNE);
        printf("\nКонец\n");
    }

    MPI_Finalize();
    return 0;
}
