using System;
using Lab3.Models;

namespace Lab3.BinaryTree
{
    public class BinaryTreeWithAircrafts
    {
        public BinaryTreeNode<Aircraft> RootNode { get; set; }

        public void Insert(Aircraft value)
        {
            InsertNode(new BinaryTreeNode<Aircraft>(value));
        }

        public void InsertNode(BinaryTreeNode<Aircraft> node)
        {
            if (RootNode == null)
            {
                RootNode = node;
                return;
            }

            var currentNode = RootNode;

            while (true)
            {
                if (currentNode.Value == node.Value)
                {
                    return;
                }

                if (node.Value.Price > currentNode.Value.Price)
                {
                    if (currentNode.Right == null)
                    {
                        currentNode.Right = node;
                        return;
                    }

                    currentNode = currentNode.Right;
                }
                else if (node.Value.Price < currentNode.Value.Price)
                {
                    if (currentNode.Left == null)
                    {
                        currentNode.Left = node;
                        return;
                    }

                    currentNode = currentNode.Left;
                }
            }
        }

        public void LeftRootRight()
        {
            Console.WriteLine();
            Console.WriteLine("LeftRootRight");

            LeftRootRight(RootNode);
        }

        private static void LeftRootRight(BinaryTreeNode<Aircraft> node)
        {
            if (node.Left != null)
            {
                LeftRootRight(node.Left);
            }

            Console.WriteLine($"{node.Value} ");

            if (node.Right != null)
            {
                LeftRootRight(node.Right);
            }
        }

        public void RightRootLeft()
        {
            Console.WriteLine();
            Console.WriteLine("RightRootLeft");

            RightRootLeft(RootNode);
        }

        private static void RightRootLeft(BinaryTreeNode<Aircraft> node)
        {
            if (node.Right != null)
            {
                RightRootLeft(node.Right);
            }

            Console.WriteLine($"{node.Value} ");

            if (node.Left != null)
            {
                RightRootLeft(node.Left);
            }
        }

        public void RootLeftRight()
        {
            Console.WriteLine();
            Console.WriteLine("RootLeftRight");

            RootLeftRight(RootNode);
        }

        private static void RootLeftRight(BinaryTreeNode<Aircraft> node)
        {
            Console.WriteLine($"{node.Value} ");

            if (node.Left != null)
            {
                RootLeftRight(node.Left);
            }

            if (node.Right != null)
            {
                RootLeftRight(node.Right);
            }
        }


        public void LeftRightRoot()
        {
            Console.WriteLine();
            Console.WriteLine("LeftRightRoot");

            LeftRightRoot(RootNode);
        }

        private static void LeftRightRoot(BinaryTreeNode<Aircraft> node)
        {
            if (node.Left != null)
            {
                LeftRightRoot(node.Left);
            }

            if (node.Right != null)
            {
                LeftRightRoot(node.Right);
            }

            Console.WriteLine($"{node.Value} ");
        }
        
        public Aircraft Find(int price)
        {
            var currentNode = RootNode;

            while (currentNode.Value.Price != price)
            {
                if (price > currentNode.Value.Price)
                {
                    if (currentNode.Right != null)
                    {
                        currentNode = currentNode.Right;
                    }
                }
                else if (price < currentNode.Value.Price)
                {
                    if (currentNode.Left != null)
                    {
                        currentNode = currentNode.Left;
                    }
                }
            }

            return currentNode.Value;
        }
    }
}