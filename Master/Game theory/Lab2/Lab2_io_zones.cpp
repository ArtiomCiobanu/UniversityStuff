// mpiCC -o Lab2_io Lab2_io_zones.cpp
//
// Шаг 1. Создать файл с данными (один раз):
//   mpirun -np 4 ./Lab2_io 8 0 data.bin gen
//   Режимы: 0 - случайные матрицы n x n, 1 - Пример 6.2 (3x3), 2 - Пример 6.1 (6x6)
//
// Шаг 2. Запуск на готовом файле (n берется из заголовка файла):
//   mpirun -np 4 ./Lab2_io 0 0 data.bin
//
// Формат файла (бинарный):
//   int n, int 0 (резерв, заголовок 8 байт)
//   A: n*n double по строкам
//   B: n*n double по строкам
//
// Каждый процесс читает только свою зону обеих матриц через MPI-IO:
// MPI_Type_create_subarray задает тип "зона внутри файла", MPI_File_set_view
// задает видимость файла для процесса, MPI_File_read_all читает зону коллективно.
// Процесс 0 не раздает данные другим процессам.
//
// Проверка перебором выполняется на процессе 0 только если n <= 1000.
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

#define HEADER_BYTES 8

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

// ========== ЗОНЫ ==========

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
 * Читает зону из файла коллективно.
 * disp - смещение начала матрицы в файле (в байтах)
 * n - размер глобальной матрицы n x n
 */
static void read_zone(MPI_File fh, MPI_Offset disp, int n, const Zone *z, double *buf)
{
    int cnt = z->rows * z->cols;

    if (cnt > 0)
    {
        int sizes[2] = {n, n};
        int subsizes[2] = {z->rows, z->cols};
        int starts[2] = {z->row_start, z->col_start};
        MPI_Datatype filetype;

        MPI_Type_create_subarray(2, sizes, subsizes, starts, MPI_ORDER_C, MPI_DOUBLE, &filetype);
        MPI_Type_commit(&filetype);

        MPI_File_set_view(fh, disp, MPI_DOUBLE, filetype, "native", MPI_INFO_NULL);
        MPI_File_read_all(fh, buf, cnt, MPI_DOUBLE, MPI_STATUS_IGNORE);

        MPI_Type_free(&filetype);
    }
    else
    {
        // Пустая зона (возможно при n < числа процессов): участвуем в коллективном вызове
        MPI_File_set_view(fh, disp, MPI_DOUBLE, MPI_DOUBLE, "native", MPI_INFO_NULL);
        MPI_File_read_all(fh, buf, 0, MPI_DOUBLE, MPI_STATUS_IGNORE);
    }
}

// ========== ВСПОМОГАТЕЛЬНЫЕ ==========

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
 * Последовательный перебор всех ситуаций (для проверки).
 * isNE[i*n+j] = 1, если (i,j) - равновесие Нэша.
 */
static int serial_nash(int n, const double *A, const double *B, int *isNE)
{
    int count = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int ok = 1;
            for (int k = 0; k < n && ok; k++)
            {
                if (A[k * n + j] > A[i * n + j])
                    ok = 0;
            }
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
    int gen = 0;
    const char *fname = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtask);

    // ========== АРГУМЕНТЫ ==========

    if (argc < 4)
    {
        if (myrank == root)
            fprintf(stderr, "Использование: %s <n> <режим 0|1|2> <файл> [gen]\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    fname = argv[3];
    gen = (argc > 4 && strcmp(argv[4], "gen") == 0);

    // ========== ГЕНЕРАЦИЯ ФАЙЛА (процесс 0) ==========

    if (myrank == root)
    {
        if (gen)
        {
            char *end = NULL;
            long req_n = strtol(argv[1], &end, 10);
            if (end == argv[1] || *end != '\0' || req_n < 1 || req_n > 2000)
            {
                fprintf(stderr, "Размер матрицы: целое число от 1 до 2000.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            n = (int)req_n;

            end = NULL;
            long req_mode = strtol(argv[2], &end, 10);
            if (end == argv[2] || *end != '\0' || req_mode < 0 || req_mode > 2)
            {
                fprintf(stderr, "Режим: 0 - случайные, 1 - пример 6.2, 2 - пример 6.1.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            mode = (int)req_mode;

            if (mode == 1)
                n = 3;
            if (mode == 2)
                n = 6;

            double *A = (double *)malloc((size_t)n * n * sizeof(double));
            double *B = (double *)malloc((size_t)n * n * sizeof(double));
            if (!A || !B)
            {
                fprintf(stderr, "Ошибка выделения памяти.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }

            fill_matrices(mode, n, A, B);

            FILE *f = fopen(fname, "wb");
            if (!f)
            {
                fprintf(stderr, "Не удалось создать файл %s\n", fname);
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            int header[2] = {n, 0};
            fwrite(header, sizeof(int), 2, f);
            fwrite(A, sizeof(double), (size_t)n * n, f);
            fwrite(B, sizeof(double), (size_t)n * n, f);
            fclose(f);

            free(A);
            free(B);
            printf("Файл %s создан: n=%d, режим=%d\n", fname, n, mode);
        }
        else
        {
            // Читаем n из заголовка
            int header[2] = {0, 0};
            FILE *f = fopen(fname, "rb");
            if (!f || fread(header, sizeof(int), 2, f) != 2)
            {
                fprintf(stderr, "Не удалось прочитать заголовок файла %s\n", fname);
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            if (f)
                fclose(f);
            n = header[0];
            if (n < 1)
            {
                fprintf(stderr, "Некорректный заголовок файла.\n");
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }
    }

    // Все процессы должны получить n и видеть полностью записанный файл
    MPI_Bcast(&n, 1, MPI_INT, root, MPI_COMM_WORLD);

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
        printf("\n===== Программа '%s' (чтение зон из файла, n=%d) =====\n", argv[0], n);
        printf("Файл: %s\n", fname);
        printf("Количество процессов: %d (%dx%d сетка)\n", numtask, proc_grid_rows, proc_grid_cols);
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

    // ========== ЧТЕНИЕ СВОЕЙ ЗОНЫ (параллельный ввод) ==========

    MPI_File fh;
    if (MPI_File_open(MPI_COMM_WORLD, fname, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh) != MPI_SUCCESS)
    {
        fprintf(stderr, "Процесс %d: не удалось открыть файл %s\n", myrank, fname);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double t_io0 = MPI_Wtime();

    MPI_Offset offA = HEADER_BYTES;
    MPI_Offset offB = HEADER_BYTES + (MPI_Offset)n * n * sizeof(double);

    read_zone(fh, offA, n, &my_zone, localA);
    read_zone(fh, offB, n, &my_zone, localB);

    MPI_File_close(&fh);
    double io_time = MPI_Wtime() - t_io0;

    // ========== ПАРАЛЛЕЛЬНАЯ ЧАСТЬ ==========

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    // Шаг 1. Максимумы по столбцам A (по всем строкам)
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

    // Шаг 2. Максимумы по строкам B (по всем столбцам)
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

    // Шаг 3. Проверка клеток своей зоны
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

    // Шаг 4. Сбор равновесий на процессе 0
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
            recvcounts[r] = 2 * counts[r];
            displs[r] = 2 * totalNE;
            totalNE += counts[r];
        }
        allNE = (Pair *)malloc((totalNE > 0 ? totalNE : 1) * sizeof(Pair));
    }

    MPI_Gatherv(localNE, 2 * localNECount, MPI_INT,
                (int *)allNE, recvcounts, displs, MPI_INT,
                root, MPI_COMM_WORLD);

    double compute_time = MPI_Wtime() - t0;

    double max_io = 0.0, max_compute = 0.0;
    MPI_Reduce(&io_time, &max_io, 1, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Reduce(&compute_time, &max_compute, 1, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);

    // ========== ВЫВОД ЗОН (по очереди, только для малых n) ==========

    for (int r = 0; r < numtask; r++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (myrank == r && n <= 10)
        {
            printf("Процесс %d: строки %d-%d, столбцы %d-%d, равновесий в зоне: %d\n",
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

        printf("\n===== Равновесия Нэша в чистых стратегиях =====\n");
        if (totalNE == 0)
        {
            printf("Равновесий в чистых стратегиях нет.\n");
        }
        else
        {
            printf("Найдено равновесий: %d\n", totalNE);
        }
        for (int k = 0; k < totalNE; k++)
        {
            printf("(%d,%d)\n", allNE[k].i, allNE[k].j);
        }

        // Проверка последовательным перебором (только для небольших n)
        if (n <= 1000)
        {
            double *fA = (double *)malloc((size_t)n * n * sizeof(double));
            double *fB = (double *)malloc((size_t)n * n * sizeof(double));
            FILE *f = fopen(fname, "rb");
            int ok_read = (f != NULL) &&
                          (fseek(f, HEADER_BYTES, SEEK_SET) == 0) &&
                          (fread(fA, sizeof(double), (size_t)n * n, f) == (size_t)n * n) &&
                          (fread(fB, sizeof(double), (size_t)n * n, f) == (size_t)n * n);
            if (f)
                fclose(f);

            if (ok_read)
            {
                int *isNE = (int *)malloc((size_t)n * n * sizeof(int));
                int *parFlag = (int *)calloc((size_t)n * n, sizeof(int));
                int serialCount = serial_nash(n, fA, fB, isNE);

                for (int k = 0; k < totalNE; k++)
                    parFlag[allNE[k].i * n + allNE[k].j] = 1;

                int mismatch = 0;
                for (int k = 0; k < n * n; k++)
                    if (parFlag[k] != isNE[k])
                        mismatch = 1;

                printf("\nПроверка перебором: %d равновесий - %s\n",
                       serialCount,
                       (!mismatch && serialCount == totalNE) ? "СОВПАДАЕТ" : "НЕ СОВПАДАЕТ");

                free(isNE);
                free(parFlag);
            }
            else
            {
                printf("\nПроверка перебором пропущена: не удалось прочитать файл.\n");
            }

            free(fA);
            free(fB);
        }
        else
        {
            printf("\nПроверка перебором пропущена (n > 1000).\n");
        }

        printf("\nВремя чтения файла (max по процессам): %.6f с\n", max_io);
        printf("Время вычислений (max по процессам):   %.6f с\n", max_compute);
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
        free(counts);
        free(recvcounts);
        free(displs);
        free(allNE);
        printf("\nКонец\n");
    }

    MPI_Finalize();
    return 0;
}
