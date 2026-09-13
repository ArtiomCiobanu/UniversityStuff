package com.company;

import java.util.Scanner;

class Matrix implements IMatrix
{
    public final int N;
    public final int M;

    private final int[][] MatrixData;

    public Matrix(int n, int m)
    {
        N = n;
        M = m;

        MatrixData = new int[n][m];
    }

    public void InitializeFromKeyboard()
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                var s = new Scanner(System.in);
                MatrixData[i][j] = s.nextInt();
            }
        }
    }

    public void PrintMatrix()
    {

        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < M; j++)
            {
                System.out.print(MatrixData[i][j] + " ");
            }
            System.out.println();
        }
    }

    @Override
    public String toString()
    {
        return String.format("%s X %s", N, M);
    }
}