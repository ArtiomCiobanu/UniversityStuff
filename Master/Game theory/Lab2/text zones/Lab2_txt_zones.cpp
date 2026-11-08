// mpiCC -o Lab2_txt Lab2_txt_fileonly.cpp
//
// Запуск (n читается из первой строки файла):
//   mpirun -np 4 ./Lab2_txt <файл>
//
// Формат текстового файла (фиксированная ширина полей):
//   строка 1: n, выровненное по правому краю в 10 символах, затем '\n' (11 байт)
//   далее n строк матрицы A: каждый элемент - поле из 12 символов "%12.4f", затем '\n'
//   далее n строк матрицы B в том же формате
//
// Каждый процесс читает только строки своей зоны (MPI_File_read_at).
// Проверка перебором выполняется на процессе 0 только если n <= 1000.
//
// Индексы в выводе - с нуля, как в методичке.

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>

#define HEADER_BYTES 11   // "%10d\n"
#define FIELD_W 12        // "%12.4f"

// ========== СТРУКТУРЫ ==========

typedef struct
{
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

            rank++;
        }
    }

    return zones;
}

/**
 * Читает свою зону из текстового файла.
 * base - смещение первой строки матрицы (в байтах)
 */
static void read_zone_txt(MPI_File fh, int n, const Zone *z, MPI_Offset base, double *out)
{
    int cols = z->cols;
    if (z->rows == 0 || cols == 0)
        return;

    MPI_Offset row_len = (MPI_Offset)n * FIELD_W + 1; // n полей + '\n'
    int bytes = cols * FIELD_W;

    char *buf = (char *)malloc(bytes + 1);
    if (!buf)
    {
        fprintf(stderr, "Ошибка выделения памяти для чтения строки.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    char tmp[FIELD_W + 1];

    for (int i = 0; i < z->rows; i++)
    {
        MPI_Offset off = base
                       + (MPI_Offset)(z->row_start + i) * row_len
                       + (MPI_Offset)z->col_start * FIELD_W;

        MPI_File_read_at(fh, off, buf, bytes, MPI_CHAR, MPI_STATUS_IGNORE);

        for (int j = 0; j < cols; j++)
        {
            memcpy(tmp, buf + j * FIELD_W, FIELD_W);
            tmp[FIELD_W] = '\0';
            out[i * cols + j] = strtod(tmp, NULL);
        }
    }

    free(buf);
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
    const char *fname = NULL;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &numtask);

    // ========== АРГУМЕНТЫ ==========

    if (argc < 2)
    {
        if (myrank == root)
            fprintf(stderr, "Использование: %s <файл>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }

    fname = argv[1];

    // ========== ЧТЕНИЕ n ИЗ ЗАГОЛОВКА (процесс 0) ==========

    if (myrank == root)
    {
        FILE *f = fopen(fname, "r");
        if (!f || fscanf(f, "%d", &n) != 1)
        {
            fprintf(stderr, "Не удалось прочитать заголовок файла %s\n", fname);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fclose(f);
        if (n < 1)
        {
            fprintf(stderr, "Некорректный заголовок файла.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, root, MPI_COMM_WORLD);

    // ========== СЕТКА ПРОЦЕССОВ ==========

    int proc_grid_rows = (int)sqrt((double)numtask);
    int proc_grid_cols = numtask / proc_grid_rows;

    if (proc_grid_rows * proc_grid_cols != numtask)
    {
        if (myrank == root)
        {
            fprintf(stderr, "Ошибка: количество процессов (%d) не подходит для 2D сетки.\n", numtask);
            fprintf(stderr, "Количество процессов должно делиться на floor(sqrt(p)), например 1, 2, 3, 4, 6, 8, 9, 12, 16.\n");
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

    MPI_Offset rowLen = (MPI_Offset)n * FIELD_W + 1;
    MPI_Offset baseA = HEADER_BYTES;
    MPI_Offset baseB = HEADER_BYTES + (MPI_Offset)n * rowLen;

    MPI_Barrier(MPI_COMM_WORLD);
    double t_io0 = MPI_Wtime();

    read_zone_txt(fh, n, &my_zone, baseA, localA);
    read_zone_txt(fh, n, &my_zone, baseB, localB);

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
            int ok_read = 1;

            FILE *f = fopen(fname, "r");
            int nf = 0;
            if (!f || fscanf(f, "%d", &nf) != 1 || nf != n)
                ok_read = 0;

            for (size_t k = 0; ok_read && k < (size_t)n * n; k++)
                if (fscanf(f, "%lf", &fA[k]) != 1)
                    ok_read = 0;
            for (size_t k = 0; ok_read && k < (size_t)n * n; k++)
                if (fscanf(f, "%lf", &fB[k]) != 1)
                    ok_read = 0;

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