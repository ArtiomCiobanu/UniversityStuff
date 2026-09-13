using System;
using System.IO;
using System.Linq;
using Lab1.BinaryTreeItems;
using Lab1.Models;
using Newtonsoft.Json;

namespace Lab1
{
    internal class Program
    {
        private static Aircraft[] GetAircraftsFromFile(string fileName)
        {
            var fileText = File.ReadAllText(fileName);

            return JsonConvert.DeserializeObject<Aircraft[]>(fileText);
        }

        private static void Main()
        {
            var aircrafts = GetAircraftsFromFile("Aircrafts.json");
            //Поиск через бинарное дерево
            var tree = new BinaryTree(aircrafts.First());
            foreach (var aircraft in aircrafts)
            {
                tree.InsertValue(aircraft);
            }

            var result = tree.Find(2940).ToArray();

            Console.WriteLine($"Поиск через бинарное дерево: {result.FirstOrDefault()}");
        }

        public static void All()
        {
            var aircrafts = GetAircraftsFromFile("Aircrafts.json");

            //Линейный поиск
            //var aircraftById = aircrafts.FindWhere(aircraft => aircraft.Id == 5);
            //var yak40 = aircrafts.FindByName("Yakovlev 40");
            var yak40 = aircrafts.FindWhere(x => x.Price == 2940);

            //Console.WriteLine(aircraftById);
            Console.WriteLine($"Линейный поиск(Найти самолёт Як-40): {yak40}");

            //Поиск через бинарное дерево
            var tree = new BinaryTree(aircrafts.First());
            foreach (var aircraft in aircrafts)
            {
                tree.InsertValue(aircraft);
            }

            var result = tree.Find(2940).ToArray();

            Console.WriteLine($"Поиск через бинарное дерево: {result.FirstOrDefault()}");

            //Двоичный поиск
            var sortedAircrafts = aircrafts.OrderBy(aircraft => aircraft.Price).ToArray();

            var binarySearchAircraft = sortedAircrafts.BinarySearchPrice(2940);
            var interpolationAircraft = sortedAircrafts.InterpolationSearch(2940);
            var fibonacciAircraft = sortedAircrafts.FibonacciSearch(2940);

            Console.WriteLine($"Binary поиск: {binarySearchAircraft}");
            Console.WriteLine($"Interpolation поиск: {interpolationAircraft}");
            Console.WriteLine($"Поиск алгоритмом Фибоначчи: {fibonacciAircraft}");
        }
    }
}