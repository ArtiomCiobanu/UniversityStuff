using System;

namespace Lab3.DoublyLinkedList
{
    public class DoublyLinkedList<T>
    {
        public int Length { get; private set; }

        private DoublyLinkedListNode<T> Root { get; set; }
        private DoublyLinkedListNode<T> Tail { get; set; }

        public DoublyLinkedList()
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

        private DoublyLinkedListNode<T> FindNode(int index)
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
            var newNode = new DoublyLinkedListNode<T>(value);

            if (Length == 0)
            {
                Root = newNode;
                Tail = newNode;
            }
            else
            {
                Tail.Next = newNode;
                newNode.Previous = Tail;
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

                foundNode.Previous = previous;
                foundNode = foundNode.Next;
                previous.Next = foundNode;
            }
            else
            {
                foundNode.Next = Root.Next;
                foundNode.Next.Previous = foundNode;
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