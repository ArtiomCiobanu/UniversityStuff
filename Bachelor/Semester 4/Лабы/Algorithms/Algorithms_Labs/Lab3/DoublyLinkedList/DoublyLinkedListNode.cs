namespace Lab3.DoublyLinkedList
{
    public class DoublyLinkedListNode<T>
    {
        public T Value { get; set; }

        public DoublyLinkedListNode<T> Previous { get; set; }
        public DoublyLinkedListNode<T> Next { get; set; }

        public DoublyLinkedListNode()
        {
        }

        public DoublyLinkedListNode(T value)
        {
            Value = value;
        }

        public DoublyLinkedListNode(T value, DoublyLinkedListNode<T> next) : this(value)
        {
            Next = next;
        }
    }
}