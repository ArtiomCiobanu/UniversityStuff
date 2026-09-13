using System;
using System.IO;
using System.Linq;
using Lab3.CircularList;
using Lab3.DoublyLinkedList;
using Lab3.LinkedList;
using Lab3.Models;
using Newtonsoft.Json;

namespace Lab3
{
    internal class Program
    {
        private static readonly Aircraft[] Aircrafts = GetAircraftsFromFile("Aircrafts.json");

        private static void Main()
        {
            //StackTest();
            //QueueTest();
            //LinkedListTest();
            //DoublyLinkedListTest();
            //CircularListTest();
            //BinaryTreeTest();

            SimpleBinaryTreeTest();
        }

        private static void SimpleBinaryTreeTest()
        {
            var binaryTree = new BinaryTree.BinaryTree();

            for (int i = 0; i < 20; i++)
            {
                var element = new Random().Next(20);
                binaryTree.Insert(element);
            }

            var fifteen = binaryTree.Find(15);

            Console.WriteLine(fifteen);
        }

        private static void BinaryTreeTest()
        {
            var binaryTree = new BinaryTree.BinaryTreeWithAircrafts();

            foreach (var aircraft in Aircrafts.Take(10))
            {
                binaryTree.Insert(aircraft);
            }

            binaryTree.LeftRootRight();
            binaryTree.RightRootLeft();
            binaryTree.RootLeftRight();
            binaryTree.LeftRightRoot();

            Console.WriteLine();
            Console.WriteLine("Searching for the aircraft with the price 1465:");
            Console.WriteLine(binaryTree.Find(1465));
        }

        private static void StackTest()
        {
            var aircraftsStack = new Stack<Aircraft>();
            foreach (var aircraft in Aircrafts)
            {
                aircraftsStack.Push(aircraft);
            }

            Console.WriteLine($"The length of the queue: {aircraftsStack.Length}");

            var someAircraft = aircraftsStack.Pop();

            Console.WriteLine($"The length of the queue after popping one element: {aircraftsStack.Length}");

            var anotherAircraft = aircraftsStack.Pop();

            Console.WriteLine("The last 10 elements:");

            for (int i = 0; i < 10; i++)
            {
                Console.WriteLine(aircraftsStack.Pop());
            }

            Console.WriteLine($"The length of the queue after popping 10 elements: {aircraftsStack.Length}");
        }

        private static void QueueTest()
        {
            var aircraftsQueue = new Queue<Aircraft>();
            foreach (var aircraft in Aircrafts)
            {
                aircraftsQueue.Push(aircraft);
            }

            Console.WriteLine($"The length of the stack: {aircraftsQueue.Length}");

            var someAircraft = aircraftsQueue.Pop();

            Console.WriteLine($"The length of the stack after popping one element: {aircraftsQueue.Length}");

            var anotherAircraft = aircraftsQueue.Pop();

            Console.WriteLine("The last 10 elements:");

            for (int i = 0; i < 10; i++)
            {
                Console.WriteLine(aircraftsQueue.Pop());
            }

            Console.WriteLine($"The length of the stack after popping 10 elements: {aircraftsQueue.Length}");
        }

        private static void LinkedListTest()
        {
            var linkedList = new LinkedList<Aircraft>();
            foreach (var aircraft in Aircrafts)
            {
                linkedList.Add(aircraft);
            }

            for (int i = 0; i < linkedList.Length; i++)
            {
                linkedList[i] = linkedList[i];
                Console.WriteLine($"linkedList[{i}] = {linkedList[i]}");
            }

            Console.WriteLine($"Initial length: {linkedList.Length}");
            linkedList.RemoveAt(100);
            Console.WriteLine($"Length after deletion of an element in the middle: {linkedList.Length}");

            linkedList.Clear();
            Console.WriteLine($"Length after clearing: {linkedList.Length}");
        }

        private static void DoublyLinkedListTest()
        {
            var doublyLinkedList = new DoublyLinkedList<Aircraft>();
            foreach (var aircraft in Aircrafts)
            {
                doublyLinkedList.Add(aircraft);
            }

            for (int i = 0; i < doublyLinkedList.Length; i++)
            {
                doublyLinkedList[i] = doublyLinkedList[i];
                Console.WriteLine($"doublyLinkedList[{i}] = {doublyLinkedList[i]}");
            }

            Console.WriteLine($"Initial length: {doublyLinkedList.Length}");
            doublyLinkedList.RemoveAt(100);
            Console.WriteLine($"Length after deletion of an element in the middle: {doublyLinkedList.Length}");

            doublyLinkedList.Clear();
            Console.WriteLine($"Length after clearing: {doublyLinkedList.Length}");
        }

        private static void CircularListTest()
        {
            var circularList = new CircularList<Aircraft>();
            foreach (var aircraft in Aircrafts)
            {
                circularList.Add(aircraft);
            }

            for (int i = 0; i < circularList.Length; i++)
            {
                circularList[i] = circularList[i];
                Console.WriteLine($"circularList[{i}] = {circularList[i]}");
            }

            Console.WriteLine($"Initial length: {circularList.Length}");
            circularList.RemoveAt(100);
            Console.WriteLine($"Length after deletion of an element in the middle: {circularList.Length}");

            circularList.Clear();
            Console.WriteLine($"Length after clearing: {circularList.Length}");
        }

        private static Aircraft[] GetAircraftsFromFile(string fileName)
        {
            var fileText = File.ReadAllText(fileName);

            return JsonConvert.DeserializeObject<Aircraft[]>(fileText);
        }
    }
}