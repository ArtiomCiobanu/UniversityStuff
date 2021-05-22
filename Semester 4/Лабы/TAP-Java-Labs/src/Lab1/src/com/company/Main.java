package com.company;

public class Main
{
    private final static RectangularTable[] tables =
            {
                    new RectangularTable(2, 3),
                    new RectangularTable(1, 2),
                    new RectangularTable(2.5, 1.5),
                    new RectangularTable(2, 1.5),
                    new RectangularTable(2, 8),
                    new RectangularTable(5, 3),
            };


    //Лабораторная No.1
    //Чобану Артём I1902

    //Задание:
    //Создайте иерархию классов Сторона - Прямоугольник - Прямоугольный Стол.
    // Класс Прямоугольный Стол должен содержать метод, который определяет площадь стола,
    // а также метод, который определяет может ли определенное число людей сесть за стол,
    // в предположении, что для каждого человека необходимо 0.85 м. По углам людей не рассаживать.
    // Создать метод MAIN, в котором создается 6 различных столов и определяется смогут ли за них сесть 9 человек.
    public static void main(String[] args)
    {
        for (RectangularTable table : tables)
        {
            System.out.printf("Стол с площадью: %s может вместить %s человек. ",
                    table.GetArea(), table.GetPersonAmount());
            System.out.printf("Он %s вместить 9 человек %n",
                    table.CanFitPeople(9) ? "может" : "НЕ может");
        }
    }
}
