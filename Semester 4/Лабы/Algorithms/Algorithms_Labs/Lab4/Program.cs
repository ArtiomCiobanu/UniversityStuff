using System;

namespace Lab4
{
    internal class Program
    {
        private static void Main()
        {
            Range range1 = new Range(-1, 1);
            Range range2 = new Range(2, 4);
            Range range3 = new Range(5, 6);
            Range range4 = new Range(7, 8);

            var array = new Array<int>(range1, range2, range3, range4);

            for (int i = 0; i < array.Length; i++)
            {
                array.Elements[i] = i;
            }

            for (int i1 = range1.L; i1 <= range1.H; i1++)
            {
                for (int i2 = range2.L; i2 <= range2.H; i2++)
                {
                    for (int i3 = range3.L; i3 <= range3.H; i3++)
                    {
                        for (int i4 = range4.L; i4 <= range4.H; i4++)
                        {
                            Console.Write($"[{i1},{i2},{i3},{i4}] = {array.GetValueByDefiningVector(i1, i2, i3, i4)} ");
                        }

                        Console.WriteLine();
                    }
                }
            }

            //Filling only Illiffes vector
            int n = 0;
            for (int i1 = range1.L; i1 <= range1.H; i1++)
            {
                for (int i2 = range2.L; i2 <= range2.H; i2++)
                {
                    for (int i3 = range3.L; i3 <= range3.H; i3++)
                    {
                        for (int i4 = range4.L; i4 <= range4.H; i4++)
                        {
                            //array.SetValueByIllifesVector(n, i1, i2, i3, i4);
                            //Console.Write($"[{i1},{i2},{i3},{i4}] = {array.GetValueByIllifesVector(i1, i2, i3, i4)} ");

                            array.SetValueByIlliffesVector(n, i1, i2, i3, i4);
                            Console.Write($"[{i1},{i2},{i3},{i4}] = {array.GetValueByIlliffesVector(i1, i2, i3, i4)} ");

                            n++;
                        }

                        Console.WriteLine();
                    }
                }
            }

            Console.WriteLine();

            PrintMeasuredTime(() => array.GetRowValue(-1, 2, 5, 7), "Direct access by rows");
            PrintMeasuredTime(() => array.GetColumnValue(-1, 2, 5, 7), "Direct access by columns");
            PrintMeasuredTime(() => array.GetValueByDefiningVector(-1, 2, 5, 7), "Defining vector access time");
            PrintMeasuredTime(() => array.GetValueByIlliffesVector(-1, 2, 5, 7), "Vector Illiffe access");
        }

        private static void PrintMeasuredTime(Func<int> action, string message)
        {
            var beginTime = DateTime.Now;
            action();
            var endTime = DateTime.Now;
            Console.WriteLine($"{message} {(endTime - beginTime).TotalMilliseconds} ms");
        }
    }
}