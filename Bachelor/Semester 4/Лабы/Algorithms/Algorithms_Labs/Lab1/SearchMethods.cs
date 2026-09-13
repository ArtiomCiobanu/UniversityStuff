using System;
using System.Collections.Generic;
using Lab1.Models;

namespace Lab1
{
    public static class SearchMethods
    {
        public static T FindWhere<T>(this IEnumerable<T> collection, Predicate<T> condition)
            where T : class
        {
            foreach (var item in collection)
            {
                if (condition(item))
                {
                    return item;
                }
            }

            return null;
        }

        public static Aircraft FindByName(this IEnumerable<Aircraft> aircrafts, string name)
            => aircrafts.FindWhere(a => a.Name == name);

        public static Aircraft BinarySearchPrice(this Aircraft[] collection, int price)
        {
            int leftIndex = 0;
            int rightIndex = collection.Length - 1;

            while (collection[leftIndex].Price != price)
            {
                var middle = (int) Math.Round((leftIndex + rightIndex) / 2.0);

                if (collection[middle].Price > price)
                {
                    rightIndex = middle;
                }
                else
                {
                    leftIndex = middle;
                }
            }

            return collection[leftIndex];
        }

        public static Aircraft InterpolationSearch(this Aircraft[] collection, int price)
        {
            int leftIndex = 0;
            int rightIndex = collection.Length - 1;

            double leftValue = collection[leftIndex].Price;
            double rightValue = collection[rightIndex].Price;

            while (leftIndex < rightIndex &&
                   leftValue <= price && price <= rightValue)
            {
                int m = (int) Math.Ceiling(leftIndex +
                                           (price - leftValue) * (rightIndex - leftIndex) /
                                           (rightValue - leftValue));

                if (price > collection[m].Price)
                {
                    leftIndex = m + 1;
                }
                else if (price < collection[m].Price)
                {
                    rightIndex = m + 1;
                }
                else
                {
                    return collection[m];
                }
 
                leftValue = collection[leftIndex].Price;
                rightValue = collection[rightIndex].Price;
            }

            return collection[leftIndex];
        }

        public static Aircraft FibonacciSearch(this Aircraft[] collection, int price)
        {
            int length = collection.Length;

            int eliminated = -1;

            int left = 0;
            int middle = 1;
            int right = middle;

            while (right < length)
            {
                left = middle;
                middle = right;
                right = left + middle;
            }

            while (middle > 1 && eliminated + left < length)
            {
                int index = eliminated + left;

                if (collection[index].Price < price)
                {
                    right = middle;
                    middle = left;
                    left = right - middle;
                    eliminated = index;
                }
                else if (collection[index].Price > price)
                {
                    right = left;
                    middle -= left;
                    left = right - middle;
                }
                else
                {
                    return collection[index];
                }
            }

            return null;
        }

        private static int Fibonacci(int n)
        {
            int num1 = 0;
            int num2 = 1;

            for (int i = 2; i <= n; i++)
            {
                int r = num1 + num2;
                num1 = num2;
                num2 = r;
            }

            return num2;
        }
    }
}