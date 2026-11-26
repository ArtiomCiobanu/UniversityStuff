//mpiCC -o Lab2 Lab2.cpp
//mpirun -host compute-0-1:4 Lab2.exe Lab2.txt

#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#define HEADER_BYTES 11   // заголовок "%10d\n"
#define FIELD_W 12        // поле числа "%12.4f"

// Равновесие с выигрышами игроков (формируется на процессе 0)
typedef struct
{
    int i;
    int j;
    double a; // выигрыш первого игрока
    double b; // выигрыш второго игрока
} Equilibrium;

// ========== РАЗБИЕНИЕ ==========

/**
 * Делит n элементов на p частей по непрерывным блокам.
 * Часть r получает элементы [*start, *start + *count).
 */
static void block_range(int n, int p, int r, int *start, int *count)
{
    int base = n / p;
    int extra = n % p;
    *start = r * base + (r < extra ? r : extra);
    *count = base + (r < extra ? 1 : 0);
}

// ========== ЧТЕНИЕ ==========

/**
 * Читает подматрицу [row0, row0+nrows) x [col0, col0+ncols) из текстового файла.
 * Результат записывается в out по строкам: out[i*ncols + j].
 */
static void read_block(MPI_File fh, int n, MPI_Offset base,
                       int row0, int nrows, int col0, int ncols, double *out)
{
    if (nrows == 0 || ncols == 0)
        return;

    MPI_Offset row_len = (MPI_Offset)n * FIELD_W + 1; // n полей + '\n'
    int bytes = ncols * FIELD_W;

    char *buf = (char *)malloc(bytes + 1);
    if (!buf)
    {
        fprintf(stderr, "Ошибка выделения памяти для чтения.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    char tmp[FIELD_W + 1];

    for (int i = 0; i < nrows; i++)
    {
        MPI_Offset off = base
                       + (MPI_Offset)(row0 + i) * row_len
                       + (MPI_Offset)col0 * FIELD_W;

        MPI_File_read_at(fh, off, buf, bytes, MPI_CHAR, MPI_STATUS_IGNORE);

        for (int j = 0; j < ncols; j++)
        {
            memcpy(tmp, buf + j * FIELD_W, FIELD_W);
            tmp[FIELD_W] = '\0';
            out[i * ncols + j] = strtod(tmp, NULL);
        }
    }

    free(buf);
}

// ========== СБОР КАНДИДАТОВ НА ПРОЦЕССЕ 0 ==========

/**
 * Собирает на процессе 0 пары (i,j) и значения от всех процессов.
 * all_ij и all_v заполняются только на процессе 0; на остальных остаются NULL.
 */
static void gather_cands(int cnt, const int *ij, const double *v, int numtask, int rank,
                         int **all_ij, double **all_v, int *total)
{
    int *counts = NULL, *ijCount = NULL, *ijDispl = NULL, *vDispl = NULL;
    *total = 0;

    if (rank == 0)
    {
        counts  = (int *)malloc(numtask * sizeof(int));
        ijCount = (int *)malloc(numtask * sizeof(int));
        ijDispl = (int *)malloc(numtask * sizeof(int));
        vDispl  = (int *)malloc(numtask * sizeof(int));
    }

    MPI_Gather(&cnt, 1, MPI_INT, counts, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        for (int r = 0; r < numtask; r++)
        {
            ijCount[r] = 2 * counts[r];
            ijDispl[r] = 2 * (*total);
            vDispl[r]  = *total;
            *total += counts[r];
        }
        *all_ij = (int *)malloc((*total > 0 ? 2 * (*total) : 1) * sizeof(int));
        *all_v  = (double *)malloc((*total > 0 ? *total : 1) * sizeof(double));
    }

    MPI_Gatherv(ij, 2 * cnt, MPI_INT, *all_ij, ijCount, ijDispl, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gatherv(v, cnt, MPI_DOUBLE, *all_v, counts, vDispl, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0)
    {
        free(counts);
        free(ijCount);
        free(ijDispl);
        free(vDispl);
    }
}

// ========== ПОСЛЕДОВАТЕЛЬНАЯ ПРОВЕРКА ==========

/**
 * Перебор всех ситуаций для проверки.
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
                if (A[k * n + j] > A[i * n + j])
                    ok = 0;
            for (int l = 0; l < n && ok; l++)
                if (B[i * n + l] > B[i * n + j])
                    ok = 0;
            isNE[i * n + j] = ok;
            count += ok;
        }
    }
    return count;
}

// ========== MAIN ==========

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

    // ========== РАЗМЕРНОСТЬ n ИЗ ЗАГОЛОВКА (процесс 0) ==========
    // n - размер матриц из файла, он не зависит от числа процессов.
    // Число процессов влияет только на то, как матрицы делятся между ними.

    if (myrank == root)
    {
        FILE *f = fopen(fname, "r");
        if (!f || fscanf(f, "%d", &n) != 1)
        {
            fprintf(stderr, "Не удалось прочитать заголовок файла %s\n", fname);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        if (f)
            fclose(f);
        if (n < 1)
        {
            fprintf(stderr, "Некорректный заголовок файла.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, root, MPI_COMM_WORLD);

    if (myrank == root)
    {
        printf("\n===== Программа '%s' (полосовое разбиение, n=%d) =====\n", argv[0], n);
        printf("Файл: %s\n", fname);
        printf("Количество процессов: %d\n", numtask);
    }

    // ========== ПОЛОСЫ ==========
    // A делим по СТОЛБЦАМ: процесс владеет полными столбцами (нужны максимумы по столбцам).
    // B делим по СТРОКАМ:  процесс владеет полными строками (нужны максимумы по строкам).

    int col_start, my_cols;
    int row_start, my_rows;
    block_range(n, numtask, myrank, &col_start, &my_cols);
    block_range(n, numtask, myrank, &row_start, &my_rows);

    // localA: n строк x my_cols столбцов, localA[i*my_cols + j]
    // localB: my_rows строк x n столбцов, localB[i*n + j]
    double *localA = (double *)malloc((size_t)(n * my_cols + 1) * sizeof(double));
    double *localB = (double *)malloc((size_t)(my_rows * n + 1) * sizeof(double));
    if (!localA || !localB)
    {
        fprintf(stderr, "Процесс %d: ошибка выделения памяти.\n", myrank);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // ========== ЧТЕНИЕ СВОЕЙ ПОЛОСЫ (параллельный ввод) ==========

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

    read_block(fh, n, baseA, 0, n, col_start, my_cols, localA);   // все строки, свои столбцы
    read_block(fh, n, baseB, row_start, my_rows, 0, n, localB);   // свои строки, все столбцы

    MPI_File_close(&fh);
    double io_time = MPI_Wtime() - t_io0;

    // ========== ВЫВОД ПРОЧИТАННЫХ ДАННЫХ (по очереди, для n <= 10) ==========

    if (myrank == root && n > 10)
        printf("\nВывод прочитанных данных пропущен (n > 10).\n");

    for (int r = 0; r < numtask; r++)
    {
        MPI_Barrier(MPI_COMM_WORLD);
        if (myrank == r && n <= 10)
        {
            if (my_cols > 0)
                printf("\nПроцесс %d: столбцы A [%d..%d]", myrank, col_start, col_start + my_cols - 1);
            else
                printf("\nПроцесс %d: столбцы A - пусто", myrank);

            if (my_rows > 0)
                printf(", строки B [%d..%d]\n", row_start, row_start + my_rows - 1);
            else
                printf(", строки B - пусто\n");

            printf("  A (полоса столбцов):\n");
            for (int i = 0; i < n && my_cols > 0; i++)
            {
                printf("    ");
                for (int j = 0; j < my_cols; j++)
                    printf("%10.4f", localA[i * my_cols + j]);
                printf("\n");
            }

            printf("  B (полоса строк):\n");
            for (int i = 0; i < my_rows; i++)
            {
                printf("    ");
                for (int j = 0; j < n; j++)
                    printf("%10.4f", localB[i * n + j]);
                printf("\n");
            }
            fflush(stdout);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    // ========== ПАРАЛЛЕЛЬНАЯ ЧАСТЬ ==========

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    // Шаг 1. Максимумы по столбцам A (каждый процесс считает свои полные столбцы)
    double *colMaxLocal  = (double *)malloc(n * sizeof(double));
    double *colMaxGlobal = (double *)malloc(n * sizeof(double));
    for (int j = 0; j < n; j++)
        colMaxLocal[j] = -DBL_MAX;

    for (int j = 0; j < my_cols; j++)
    {
        double m = -DBL_MAX;
        for (int i = 0; i < n; i++)
        {
            double v = localA[i * my_cols + j];
            if (v > m)
                m = v;
        }
        colMaxLocal[col_start + j] = m;
    }
    MPI_Allreduce(colMaxLocal, colMaxGlobal, n, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    // Шаг 2. Максимумы по строкам B (каждый процесс считает свои полные строки)
    double *rowMaxLocal  = (double *)malloc(n * sizeof(double));
    double *rowMaxGlobal = (double *)malloc(n * sizeof(double));
    for (int i = 0; i < n; i++)
        rowMaxLocal[i] = -DBL_MAX;

    for (int i = 0; i < my_rows; i++)
    {
        double m = -DBL_MAX;
        for (int j = 0; j < n; j++)
        {
            double v = localB[i * n + j];
            if (v > m)
                m = v;
        }
        rowMaxLocal[row_start + i] = m;
    }
    MPI_Allreduce(rowMaxLocal, rowMaxGlobal, n, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);

    // Шаг 3. Кандидаты.
    // Клетка (i,j) - кандидат по A, если A[i][j] = max по столбцу j.
    // Этим владеет процесс, у которого есть столбец j.
    int cap = n * my_cols + 1;
    int *candAij = (int *)malloc(2 * cap * sizeof(int));
    double *candAv = (double *)malloc(cap * sizeof(double));
    int cntA = 0;

    for (int j = 0; j < my_cols; j++)
    {
        int gj = col_start + j;
        for (int i = 0; i < n; i++)
        {
            double a = localA[i * my_cols + j];
            if (a == colMaxGlobal[gj])
            {
                candAij[2 * cntA]     = i;
                candAij[2 * cntA + 1] = gj;
                candAv[cntA]          = a;
                cntA++;
            }
        }
    }

    // Клетка (i,j) - кандидат по B, если B[i][j] = max по строке i.
    // Этим владеет процесс, у которого есть строка i.
    cap = my_rows * n + 1;
    int *candBij = (int *)malloc(2 * cap * sizeof(int));
    double *candBv = (double *)malloc(cap * sizeof(double));
    int cntB = 0;

    for (int i = 0; i < my_rows; i++)
    {
        int gi = row_start + i;
        for (int j = 0; j < n; j++)
        {
            double b = localB[i * n + j];
            if (b == rowMaxGlobal[gi])
            {
                candBij[2 * cntB]     = gi;
                candBij[2 * cntB + 1] = j;
                candBv[cntB]          = b;
                cntB++;
            }
        }
    }

    // Шаг 4. Сбор кандидатов на процессе 0
    int *allAij = NULL, *allBij = NULL;
    double *allAv = NULL, *allBv = NULL;
    int totA = 0, totB = 0;

    gather_cands(cntA, candAij, candAv, numtask, myrank, &allAij, &allAv, &totA);
    gather_cands(cntB, candBij, candBv, numtask, myrank, &allBij, &allBv, &totB);

    double compute_time = MPI_Wtime() - t0;

    double max_io = 0.0, max_compute = 0.0;
    MPI_Reduce(&io_time, &max_io, 1, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);
    MPI_Reduce(&compute_time, &max_compute, 1, MPI_DOUBLE, MPI_MAX, root, MPI_COMM_WORLD);

    // ========== СЧЁТЧИКИ КАНДИДАТОВ (печатает только процесс 0) ==========

    int myCnt[2] = {cntA, cntB};
    int *allCnt = NULL;
    if (myrank == root)
        allCnt = (int *)malloc(2 * numtask * sizeof(int));

    MPI_Gather(myCnt, 2, MPI_INT, allCnt, 2, MPI_INT, root, MPI_COMM_WORLD);

    if (myrank == root && n <= 10)
    {
        for (int r = 0; r < numtask; r++)
            printf("Процесс %d: кандидатов по A: %d, по B: %d\n", r, allCnt[2 * r], allCnt[2 * r + 1]);
    }

    // ========== РЕЗУЛЬТАТЫ (процесс 0) ==========

    if (myrank == root)
    {
        size_t N = (size_t)n * n;

        int *hasA = (int *)calloc(N, sizeof(int));
        int *hasB = (int *)calloc(N, sizeof(int));
        double *valA = (double *)malloc(N * sizeof(double));
        double *valB = (double *)malloc(N * sizeof(double));

        for (int k = 0; k < totA; k++)
        {
            size_t idx = (size_t)allAij[2 * k] * n + allAij[2 * k + 1];
            hasA[idx] = 1;
            valA[idx] = allAv[k];
        }
        for (int k = 0; k < totB; k++)
        {
            size_t idx = (size_t)allBij[2 * k] * n + allBij[2 * k + 1];
            hasB[idx] = 1;
            valB[idx] = allBv[k];
        }

        // Равновесие - клетка, которая является кандидатом и по A, и по B.
        // Обход по строкам дает отсортированный по (i,j) список.
        Equilibrium *eqs = (Equilibrium *)malloc(N * sizeof(Equilibrium));
        int totalNE = 0;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                size_t idx = (size_t)i * n + j;
                if (hasA[idx] && hasB[idx])
                {
                    eqs[totalNE].i = i;
                    eqs[totalNE].j = j;
                    eqs[totalNE].a = valA[idx];
                    eqs[totalNE].b = valB[idx];
                    totalNE++;
                }
            }
        }

        printf("\n===== Равновесия Нэша в чистых стратегиях =====\n");
        if (totalNE == 0)
        {
            printf("Равновесий в чистых стратегиях нет.\n");
        }
        else
        {
            printf("Найдено равновесий: %d\n", totalNE);
            printf("%-10s %14s %14s\n", "(i,j)", "P1", "P2");
            for (int k = 0; k < totalNE; k++)
            {
                char idx[24];
                snprintf(idx, sizeof(idx), "(%d,%d)", eqs[k].i, eqs[k].j);
                printf("%-10s %14.4f %14.4f\n", idx, eqs[k].a, eqs[k].b);
            }
        }

        // Проверка перебором (только для небольших n)
        if (n <= 1000)
        {
            double *fA = (double *)malloc(N * sizeof(double));
            double *fB = (double *)malloc(N * sizeof(double));
            int ok_read = 1;

            FILE *f = fopen(fname, "r");
            int nf = 0;
            if (!f || fscanf(f, "%d", &nf) != 1 || nf != n)
                ok_read = 0;

            for (size_t k = 0; ok_read && k < N; k++)
                if (fscanf(f, "%lf", &fA[k]) != 1)
                    ok_read = 0;
            for (size_t k = 0; ok_read && k < N; k++)
                if (fscanf(f, "%lf", &fB[k]) != 1)
                    ok_read = 0;

            if (f)
                fclose(f);

            if (ok_read)
            {
                int *isNE = (int *)malloc(N * sizeof(int));
                int serialCount = serial_nash(n, fA, fB, isNE);

                int mismatch = 0;
                for (size_t k = 0; k < N; k++)
                {
                    int parFlag = hasA[k] && hasB[k];
                    if (parFlag != isNE[k])
                        mismatch = 1;
                }

                printf("\nПроверка перебором: %d равновесий - %s\n",
                       serialCount,
                       (!mismatch && serialCount == totalNE) ? "СОВПАДАЕТ" : "НЕ СОВПАДАЕТ");

                free(isNE);
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

        free(hasA);
        free(hasB);
        free(valA);
        free(valB);
        free(eqs);
        free(allAij);
        free(allAv);
        free(allBij);
        free(allBv);
        free(allCnt);
    }

    // ========== ОСВОБОЖДЕНИЕ ПАМЯТИ ==========

    free(localA);
    free(localB);
    free(colMaxLocal);
    free(colMaxGlobal);
    free(rowMaxLocal);
    free(rowMaxGlobal);
    free(candAij);
    free(candAv);
    free(candBij);
    free(candBv);

    if (myrank == root)
        printf("\nКонец\n");

    MPI_Finalize();
    return 0;
}