namespace Lab3.CircularList
{
    public class CircularListNode<T>
    {
        public T Value { get; set; }

        public CircularListNode<T> Previous { get; set; }
        public CircularListNode<T> Next { get; set; }

        public CircularListNode(T value)
        {
            Value = value;
        }
    }
}