using System;

namespace Lab3
{
    public class Stack<T>
    {
        private const int DefaultCapacity = 10;

        private T[] _array;

        private int _capacity;

        public int Length { get; private set; }

        public Stack()
        {
            _array = new T[DefaultCapacity];
            _capacity = DefaultCapacity;
        }

        public void Push(T value)
        {
            if (Length >= _capacity)
            {
                _capacity += 10;

                var newArray = new T[_capacity];
                for (int i = 0; i < Length; i++)
                {
                    newArray[i] = _array[i];
                }

                _array = newArray;

                _array[Length] = value;
                Length++;
            }

            _array[Length] = value;
            Length++;
        }

        public T Pop()
        {
            if (Length > 0)
            {
                Length--;

                return _array[Length];
            }
            else
            {
                throw new IndexOutOfRangeException();
            }
        }

        public void Clear()
        {
            Array.Clear(_array, 0, Length);
        }

        public T this[int index] => _array[index];
    }
}