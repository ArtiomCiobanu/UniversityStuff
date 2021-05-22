using System;

namespace Lab3.LinkedList
{
    public class LinkedList<T>
    {
        public int Length { get; private set; }

        private LinkedListNode<T> Root { get; set; }
        private LinkedListNode<T> Tail { get; set; }

        public LinkedList()
        {
            Length = 0;
        }

        public T this[int index]
        {
            get => FindNode(index).Value;
            set
            {
                var foundNode = FindNode(index);
                foundNode.Value = value;
            }
        }

        public LinkedListNode<T> FindNode(int index)
        {
            if (index < 0 || index > Length)
            {
                throw new IndexOutOfRangeException();
            }

            var currentNode = Root;
            for (int i = 1; i <= index; i++)
            {
                currentNode = currentNode.Next;
            }

            return currentNode;
        }

        public void Add(T value)
        {
            var newNode = new LinkedListNode<T>(value);

            if (Length == 0)
            {
                Root = newNode;
                Tail = newNode;
            }
            else
            {
                Tail.Next = newNode;
                Tail = newNode;
            }

            Length++;
        }

        public void RemoveAt(int index)
        {
            var foundNode = FindNode(index);
            if (index > 0)
            {
                var previous = FindNode(index - 1);

                foundNode = foundNode.Next;
                previous.Next = foundNode;
            }
            else
            {
                Root = foundNode;
            }

            Length--;
        }

        public void Clear()
        {
            Length = 0;
            Root = null;
            Tail = null;
        }
    }
}