using System;

namespace Lab3
{
    public class Queue<T>
    {
        private const int DefaultCapacity = 10;

        private T[] _array;

        private int _capacity;

        public int Length { get; private set; }

        public Queue()
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

                var result = _array[0];

                for (int i = 0; i < Length; i++)
                {
                    _array[i] = _array[i + 1];
                }

                return result;
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