using System;
using System.Collections.Generic;
using System.Linq;

namespace Attestation1
{
    public static class Extensions
    {
        public static void Display<T>(this IEnumerable<T> collection)
        {
            foreach (var element in collection)
            {
                Console.WriteLine(element);
            }

            Console.WriteLine();
        }

        public static void DisplayInline<T>(this IEnumerable<T> collection)
        {
            foreach (var element in collection)
            {
                Console.Write($"{element} ");
            }

            Console.WriteLine();
        }

        public static int BinarySearch(this char[] collection, char character)
        {
            int leftIndex = 0;
            int rightIndex = collection.Length - 1;

            while (collection[leftIndex] != character)
            {
                var middle = (int) Math.Round((leftIndex + rightIndex) / 2.0);

                if (collection[middle] > character)
                {
                    rightIndex = middle;
                }
                else
                {
                    leftIndex = middle;
                }
            }

            return leftIndex;
        }

        public static char[] BubbleSort(this char[] collection)
        {
            for (int i = 0; i < collection.Length; i++)
            {
                for (int j = 1; j < collection.Length; j++)
                {
                    if (collection[j] < collection[j - 1])
                    {
                        var c = collection[j];
                        collection[j] = collection[j - 1];
                        collection[j - 1] = c;
                    }
                }
            }

            return collection;
        }

        public static char[] BubbleSortWithDisplaying(this char[] collection)
        {
            for (int i = 0; i < collection.Length; i++)
            {
                for (int j = 1; j < collection.Length; j++)
                {
                    if (collection[j] < collection[j - 1])
                    {
                        var c = collection[j];
                        collection[j] = collection[j - 1];
                        collection[j - 1] = c;
                    }

                    Console.Write($"i = {i}  j = {j}    ");
                    collection.DisplayInline();
                }
            }

            return collection;
        }

        public static char[] InsertionSort(this char[] collection)
        {
            for (int i = 1; i < collection.Length; i++)
            {
                for (int j = i - 1; j > 0; j--)
                {
                    if (collection[i] < collection[j])
                    {
                        var current = collection[i];

                        for (int k = i; k > j; k--)
                        {
                            collection[k] = collection[k - 1];
                        }

                        collection[j] = current;
                    }

                    collection.DisplayInline();
                }
            }

            return collection;
        }

        public static char[] QuickSort(this char[] collection)
        {
            int lastIndex = collection.Length - 1;

            var areas = new List<Tuple<int, int>>
            {
                new(0, lastIndex)
            };

            while (areas.Any())
            {
                var (left, right) = areas.First();

                var pivotIndex = (right + left) / 2;
                var pivot = collection[pivotIndex];

                for (int i = pivotIndex - 1; i >= left; i--)
                {
                    if (collection[i] > pivot)
                    {
                        var current = collection[i];
                        //Сдвиг всего влево
                        for (int j = i; j < right - 1; j++)
                        {
                            collection[j] = collection[j + 1];
                        }

                        collection[right - 1] = current;

                        pivotIndex--;
                    }
                }

                for (int i = pivotIndex + 1; i < right; i++)
                {
                    if (collection[i] < pivot)
                    {
                        var current = collection[i];

                        for (int j = i; j >= left + 1; j--)
                        {
                            collection[j] = collection[j - 1];
                        }

                        collection[left] = current;

                        pivotIndex++;
                    }

                    collection.DisplayInline();
                }

                areas.RemoveAt(0);
                if (right - pivotIndex > 1)
                {
                    var area2 = new Tuple<int, int>(pivotIndex, right);
                    areas.Insert(0, area2);
                }

                if (pivotIndex - left > 1)
                {
                    var area1 = new Tuple<int, int>(left, pivotIndex);
                    areas.Insert(0, area1);
                }
            }

            return collection;
        }
        
        public static char[] SelectionSort(this char[] collection)
        {
            for (int i = 0; i < collection.Length; i++)
            {
                for (int j = i; j < collection.Length; j++)
                {
                    if (collection[j] < collection[i])
                    {
                        var c = collection[j];
                        collection[j] = collection[i];
                        collection[i] = c;
                    }
                }
            }

            return collection;
        }
    }
}