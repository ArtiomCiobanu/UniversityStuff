package com.company;

//Лабораторная No.2
//Чобану Артём I1902

//Лабораторная работа №2. Создание и обработка собственных исключений.
// В программе требуется:
//
//- Создать собственное исключение (class).
//- Создать метод (throw), который может возбуждать это исключение(throws).
//- Написать метод, перехватывающий и обрабатывающий исключение (try-catch), возбуждаемое другим методом.
//- Создать класс, который ведет себя как float, но не допускающий значений >+35767 и < -35768,
// а также значений POSITIVE_INFINITY, NEGATIVE_INFINITY, NaN вместо них записывать 33333.
// Выход за пределы значений >+35767 и < -35768, а также значение POSITIVE_INFINITY рассматривать как исключительную ситуацию.
// Использовать класс для выполнения арифметических операций (x/y x%y).
public class Main
{
    public static void main(String[] args)
    {
        FakeFloat fakeFloat1 = new FakeFloat(5);
        FakeFloat fakeFloat2 = new FakeFloat(6);

        System.out.printf("Значение 1: %s %n", fakeFloat1.Get_Value());
        System.out.printf("Значение 2: %s %n", fakeFloat2.Get_Value());

        System.out.println();
        try
        {
            fakeFloat1.AddFakeFloat(fakeFloat2);
            System.out.printf("Значение 1 + значение 2: %s %n", fakeFloat1.Get_Value());

            fakeFloat2.SubstractFakeFloat(new FakeFloat(50));
            System.out.printf("Значение 2 - 50: %s %n", fakeFloat2.Get_Value());

            fakeFloat2.DivideByFakeFloat(fakeFloat1);
            System.out.printf("Значение 2 поделённое на значение 1: %s %n", fakeFloat2.Get_Value());

            fakeFloat2.MultiplyByFakeFloat(fakeFloat1);
            System.out.printf("Значение 2 умноженное на значение 1: %s %n", fakeFloat2.Get_Value());

            fakeFloat1.Remainder(new FakeFloat(5));
            System.out.printf("Остаток от деления значения 1 на 5: %s %n", fakeFloat1.Get_Value());
        } catch (NotAllowedNumberException exception)
        {
            System.out.printf("%n Ошибка! %s %n", exception.getMessage());
        }

        System.out.println();
        System.out.println("Тестирование обработки ошибок!");
        try
        {
            System.out.println("Передача значения POSITIVE_INFINITY");
            fakeFloat1.Set_Value(Double.POSITIVE_INFINITY);
        } catch (NotAllowedNumberException exception)
        {
            System.out.printf("Ошибка! %s. Значение fakeFloat: %s %n", exception.getMessage(), fakeFloat1.Get_Value());
        }

        try
        {
            System.out.println("Передача значения -50000");
            fakeFloat1.Set_Value(-50000);
        } catch (NotAllowedNumberException exception)
        {
            System.out.printf("Ошибка! %s. Значение fakeFloat: %s %n", exception.getMessage(), fakeFloat1.Get_Value());
        }

        try
        {
            System.out.println("Передача значения 50000");
            fakeFloat1.Set_Value(50000);
        } catch (NotAllowedNumberException exception)
        {
            System.out.printf("Ошибка! %s. Значение fakeFloat: %s %n", exception.getMessage(), fakeFloat1.Get_Value());
        }
    }
}
