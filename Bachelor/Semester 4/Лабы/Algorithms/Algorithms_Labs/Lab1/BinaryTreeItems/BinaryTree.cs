using System.Collections.Generic;
using System.Linq;
using Lab1.Models;

namespace Lab1.BinaryTreeItems
{
    public class BinaryTree
    {
        private readonly BinaryTreeNode _rootNode;

        public BinaryTree(Aircraft rootAircraft)
        {
            _rootNode = new BinaryTreeNode(rootAircraft);
        }

        public void InsertNode(BinaryTreeNode node)
        {
            var currentNode = _rootNode;

            while (true)
            {
                if (currentNode.Values
                    .Select(v => v.Id)
                    .Intersect(node.Values.Select(v => v.Id))
                    .Any())
                {
                    return;
                }

                if (node.Index > currentNode.Index)
                {
                    if (currentNode.Right == null)
                    {
                        currentNode.Right = node;
                        return;
                    }

                    currentNode = currentNode.Right;
                }
                else if (node.Index < currentNode.Index)
                {
                    if (currentNode.Left == null)
                    {
                        currentNode.Left = node;
                        return;
                    }

                    currentNode = currentNode.Left;
                }
                else
                {
                    currentNode.AddRange(node.Values);
                    break;
                }
            }
        }

        public void InsertValue(Aircraft value)
        {
            InsertNode(new BinaryTreeNode(value));
        }

        public IEnumerable<Aircraft> Find(int index)
        {
            var currentNode = _rootNode;

            while (currentNode.Index != index)
            {
                if (index > currentNode.Index)
                {
                    if (currentNode.Right != null)
                    {
                        currentNode = currentNode.Right;
                    }
                }
                else if (index < currentNode.Index)
                {
                    if (currentNode.Left != null)
                    {
                        currentNode = currentNode.Left;
                    }
                }
            }

            return currentNode.Values;
        }
    }
}