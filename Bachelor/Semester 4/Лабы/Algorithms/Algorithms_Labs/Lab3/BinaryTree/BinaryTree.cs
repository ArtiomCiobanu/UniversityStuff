using System;
using System.Collections.Generic;

namespace Lab3.BinaryTree
{
    public class BinaryTree
    {
        public BinaryTreeNode<int> RootNode { get; set; }

        public void Insert(int value)
        {
            InsertNode(new BinaryTreeNode<int>(value));
        }

        public void InsertNode(BinaryTreeNode<int> node)
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

                if (node.Value > currentNode.Value)
                {
                    if (currentNode.Right == null)
                    {
                        currentNode.Right = node;
                        return;
                    }

                    currentNode = currentNode.Right;
                }
                else if (node.Value < currentNode.Value)
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

        private static void LeftRootRight(BinaryTreeNode<int> node)
        {
            if (node.Left != null)
            {
                LeftRootRight(node.Left);
            }

            Console.Write($"{node.Value} ");

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

        private static void RightRootLeft(BinaryTreeNode<int> node)
        {
            if (node.Right != null)
            {
                RightRootLeft(node.Right);
            }

            Console.Write($"{node.Value} ");

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

        private static void RootLeftRight(BinaryTreeNode<int> node)
        {
            Console.Write($"{node.Value} ");

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

        private static void LeftRightRoot(BinaryTreeNode<int> node)
        {
            if (node.Left != null)
            {
                LeftRightRoot(node.Left);
            }

            if (node.Right != null)
            {
                LeftRightRoot(node.Right);
            }

            Console.Write($"{node.Value} ");
        }

        public int Find(int value)
        {
            var currentNode = RootNode;

            while (currentNode.Value != value)
            {
                if (value > currentNode.Value)
                {
                    if (currentNode.Right != null)
                    {
                        currentNode = currentNode.Right;
                    }
                }
                else if (value < currentNode.Value)
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