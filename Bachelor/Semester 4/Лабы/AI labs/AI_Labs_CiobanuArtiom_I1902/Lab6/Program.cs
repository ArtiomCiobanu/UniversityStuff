using System;
using System.Collections.Generic;
using System.Linq;

namespace Lab1
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            Console.Write("Введите строку:");

            string input = Console.ReadLine();
            //string input = "ababcd";

            if (input.Length % 2 == 0)
            {
                var words = input.SplitBy2();
                words.Display();

                Console.WriteLine(words.IsSuitable() ? "Подходит" : "Не подходит");
            }
            else
            {
                Console.WriteLine("Не подходит");
            }
        }
    }

    internal static class Extensions
    {
        public static void Display(this IEnumerable<string> collection)
        {
            foreach (var v in collection)
            {
                Console.Write($"{v} ");
            }

            Console.WriteLine();
        }

        public static List<string> SplitBy2(this string str)
        {
            var result = new List<string>();

            for (int i = 0; i < str.Length - 1; i += 2)
            {
                result.Add(str.Substring(i, 2));
            }

            return result;
        }

        public static bool IsSuitable(this IEnumerable<string> words)
        {
            string firstWord = "ab";
            string secondWord = "cd";

            var grouping = words.GroupBy(w => w).ToList();

            if (grouping.Count == 2)
            {
                var firstCount = grouping.First(w => w.Key == firstWord).Count();
                var secondCount = grouping.First(w => w.Key == secondWord).Count();

                return firstCount == secondCount + 1;
            }

            return false;
        }
    }
}