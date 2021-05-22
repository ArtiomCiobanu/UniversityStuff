using System;
using System.Collections.Generic;
using System.Linq;
using Lab1.Models;

namespace Lab1.BinaryTreeItems
{
    public class BinaryTreeNode
    {
        public int Index { get; }

        public IList<Aircraft> Values { get; }

        public BinaryTreeNode Left { get; set; }
        public BinaryTreeNode Right { get; set; }

        public BinaryTreeNode(Aircraft value)
        {
            Index = value.Price;
            Values = new List<Aircraft>
            {
                value
            };
        }

        public void AddValue(Aircraft value)
        {
            if (value.Price == Index)
            {
                Values.Add(value);
            }
            else
            {
                throw new ArgumentException("Value has incorrect index");
            }
        }

        public void AddRange(IEnumerable<Aircraft> values)
        {
            foreach (var value in values)
            {
                AddValue(value);
            }
        }

        //public override string ToString() => Values.ToString();
        public override string ToString()
            => Values.Aggregate("",
                (current, value) =>
                    current + value + "\n");
    }
}