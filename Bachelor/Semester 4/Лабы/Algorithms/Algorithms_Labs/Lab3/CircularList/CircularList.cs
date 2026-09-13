using System;

namespace Lab3.CircularList
{
    public class CircularList<T>
    {
        public int Length { get; private set; }

        private CircularListNode<T> Root { get; set; }
        private CircularListNode<T> Tail { get; set; }

        public CircularList()
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

        private CircularListNode<T> FindNode(int index)
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
            var newNode = new CircularListNode<T>(value);

            if (Length == 0)
            {
                Root = newNode;
                Tail = newNode;

                Tail.Previous = Root;
                Root.Next = Tail;
            }
            else
            {
                Tail.Next = newNode;
                newNode.Previous = Tail;
                Tail = newNode;
            }

            Tail.Next = Root;
            Root.Previous = Tail;

            Length++;
        }

        public void RemoveAt(int index)
        {
            var foundNode = FindNode(index);
            if (index > 0)
            {
                var previous = FindNode(index - 1);

                foundNode.Previous = previous;
                foundNode = foundNode.Next;
                previous.Next = foundNode;
            }
            else
            {
                foundNode.Next = Root.Next;
                foundNode.Next.Previous = foundNode;
                Root = foundNode;

                Root.Previous = Tail;
                Tail.Next = Root;
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