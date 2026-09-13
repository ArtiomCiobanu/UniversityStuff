using System;
using System.Collections.Generic;

namespace Lab4
{
    public static class Extensions
    {
        public static void Display(this double[,] matrix, int len)
        {
            for (int i = 0; i < len; i++)
            {
                for (int j = 0; j < len; j++)
                {
                    Console.Write(matrix[i, j] + " ");
                }

                Console.WriteLine();
            }

            Console.WriteLine();
        }

        public static void Display(this IEnumerable<double> collection)
        {
            collection.DisplayInline();

            Console.WriteLine();
        }

        public static void DisplayInline(this IEnumerable<double> collection)
        {
            foreach (var t in collection)
            {
                Console.Write(t + " ");
            }
        }
    }
}