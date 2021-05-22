using System;

namespace Lab2
{
    internal class Program
    {
        private static double F(double x)
            => 1.5 - 0.4 * Math.Sqrt(Math.Pow(x, 3)) - 0.5 * Math.Log(x);

        private static double FirstDerivative(double x)
            => (-0.6 * x * Math.Sqrt(x) - 0.5) / x;

        private static double SecondDerivative(double x)
            => 0.5 * (-0.3 / Math.Sqrt(x) - 1 / (x * x));

        private static void Main()
        {
            double a = 2;
            double b = 2.5;

            var precision = Math.Pow(10000, -5);

            //Метод Деления отрезка пополам
            var iterationsAmount = (int) Math.Ceiling(Math.Log2(Math.Abs(b - a) / precision));

            Console.WriteLine(F(a) * F(b) > 0 ?
                $"На отрезке[{a}, {b}] нет корня." :
                $"Примерный ответ методом Дихотомии: {Dichotomy(a, b, iterationsAmount)}");

            Console.WriteLine($"Методом Хорд: {ChordMethod(a, b, precision)}");
            Console.WriteLine($"Методом Ньютона(Касательных): {NewtonMethod(a, b, precision)}");
            Console.WriteLine($"Методом Секущих: {SecantMethod(2.5, 3, precision)}");
        }

        private static double Dichotomy(double a, double b, int iterationsAmount)
        {
            for (int i = 0; i < iterationsAmount; i++)
            {
                var c = (a + b) / 2;

                var value = F(a) * F(c);
                if (value > 0)
                {
                    a = c;
                }
                else
                {
                    b = c;
                }
            }

            return a > b ? b : a;
        }

        private static double ChordMethod(double a, double b, double precision)
        {
            double x = a;
            if (F(a) * SecondDerivative(a) > 0)
            {
                x = b;
                b = a;
            }

            double difference = 0;
            do
            {
                difference = F(x) * (b - x) / (F(b) - F(x));

                x -= difference;
            } while (difference > precision & F(x) * F(b) < 0);

            return x;
        }

        private static double NewtonMethod(double a, double b, double precision)
        {
            double x = a;
            if (F(a) * SecondDerivative(a) > 0)
            {
                x = b;
                b = a;
            }

            double difference = 0;
            do
            {
                difference = Math.Abs(F(x) / FirstDerivative(x));
                x += difference;
            } while (difference > precision && F(x) * F(b) < 0);

            return x;
        }

        private static double SecantMethod(double a, double b, double precision)
        {
            double x = a;
            if (b > a)
            {
                x = b;
                b = a;
            }

            double difference = 0;
            do
            {
                difference = F(x) * (b - x) / (F(b) - F(x));
                x -= difference;
            } while (difference > precision);

            return x;
        }
    }
}