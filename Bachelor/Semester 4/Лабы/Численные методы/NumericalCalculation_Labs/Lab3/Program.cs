using System;

namespace Lab3
{
    internal class Program
    {
        private static int length = 4;
        
        private static double[,] A =
        {
            {2.979, 0.427, 0.406, 0.348},
            {0.273, 3.951, 0.217, 0.327},
            {0.318, 0.197, 2.875, 0.166},
            {0.219, 0.231, 0.187, 3.276}
        };

        private static double[] B =
        {
            0.341,
            0.844,
            0.131,
            0.381
        };

        private static void Main()
        {
            IterationMethod();
            //Seidel();
        }

        private static void IterationMethod()
        {
            double[,] a = new double[length, length];

            for (int i = 0; i < length; i++)
            {
                a[i, 0] = B[i] / A[i, i];

                int pos = 1;

                for (int j = 0; j < length; j++)
                {
                    if (i == j)
                    {
                        continue;
                    }

                    a[i, pos] = A[i, j] / A[i, i];
                    pos++;
                }
            }

            a.Display(length);
            Console.WriteLine();

            for (int i = 0; i < length; i++)
            {
                var c = a[i, i];
                a[i, i] = a[i, 0];
                a[i, 0] = c;
            }

            a.Display(length);

            var x = GetMainDiagonal(a);
            var e = 0.01;

            bool continueIteration = true;
            while (continueIteration)
            {
                for (int i = 0; i < length; i++)
                {
                    for (int j = 0; j < length; j++)
                    {
                        if (i == j)
                        {
                            continue;
                        }

                        a[i, i] -= a[i, j] * x[j];
                    }
                }

                var newX = GetMainDiagonal(a);

                for (int i = 0; i < length; i++)
                {
                    if (Math.Abs(x[i] - newX[i]) < e || newX[i] < 0)
                    {
                        continueIteration = false;
                    }
                }

                if (continueIteration)
                {
                    x = newX;

                    Console.WriteLine("------------------------------");
                    a.Display(length);
                    //Console.WriteLine();
                    //x.Display();
                    //newX.Display();
                }
            }

            Console.WriteLine("Ответ:");
            for (int i = 0; i < length; i++)
            {
                Console.WriteLine($"x[{i}] = {x[i]}");
            }
        }

        private static double[] GetMainDiagonal(double[,] mtx)
        {
            var x = new double[length];
            for (int i = 0; i < length; i++)
            {
                x[i] = mtx[i, i];
            }

            return x;
        }

        private static void Seidel()
        {
            //double[,] a = new double[length, length];

            //A.Display(length);

            for (int i = 0; i < length; i++)
            {
                for (int j = i; j > 0; j--)
                {
                    var saved = A[i, length - 1];
                    for (int k = length - 1; k > 0; k--)
                    {
                        A[i, k] = A[i, k - 1];
                    }

                    A[i, 0] = saved;
                }
            }

            //A.Display(length);

            for (int i = 0; i < length; i++)
            {
                for (int j = 0; j < length; j++)
                {
                    if (i == j)
                    {
                        continue;
                    }

                    A[i, j] /= A[i, i];
                }
            }

            //A.Display(length);

            double[] x = {1.0, 1, 1, 1};

            for (int i = 0; i < length; i++)
            {
                var currentX = A[i, 0] * x[0];
                for (int j = 1; j < length; j++)
                {
                    currentX -= A[i, j] * x[i];
                }

                x[i] = currentX;
            }
            
            x.Display();
        }
    }
}