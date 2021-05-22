using System;
using System.Collections.Generic;
using System.Linq;

namespace Lab6
{
    public class Program
    {
        private static void Main()
        {
            //int[] input = {3, 1, 1, 2, 2, 1};
            int[] input = {2, 1, 1, 2, 5, 1};
            input = input.OrderByDescending(x => x).ToArray();

            Console.WriteLine("Оригинальное множвество:");
            input.Display();
            Console.WriteLine("Результат:");
            
            if (input.Sum() % 2 != 0)
            {
                Console.WriteLine("Невозможно разбить на 2 подмножества с одинаковой суммой.");
                return;
            }

            var first = new List<int>();
            /*{
                input.First()
            };*/
            var second = new List<int>();
            /*{
                input.Take(2).Last()
            };*/

            foreach (var num in input)
            {
                if (first.Sum() < second.Sum())
                {
                    first.Add(num);
                }
                else
                {
                    second.Add(num);
                }
            }

            first.Display();
            second.Display();
        }
    }

    internal static class Extensions
    {
        public static void Display(this IEnumerable<int> collection)
        {
            foreach (var v in collection)
            {
                Console.Write($"{v} ");
            }

            Console.WriteLine();
        }
    }
}