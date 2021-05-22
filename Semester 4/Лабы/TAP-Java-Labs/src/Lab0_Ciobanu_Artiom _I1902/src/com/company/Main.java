package com.company;

//Лабораторная No.0
//Чобану Артём I1902

//Цель – изучение среды разработки приложений. Создание класса Java по аналогии с классами в С++.
//
//Создать класс Matrix, содержащий двумерный массив nxm целого типа,
// организовать ввод-вывод массива, поиск минимального и максимального элементов.
// Для ввода использовать класс Scanner, а для вывода метод System.out.println().
public class Main
{
    public static void main(String[] args)
    {
        IMatrix matrix = new Matrix(2, 2);

        System.out.printf("Введите данные матрицы с размерами %s", matrix);
        matrix.InitializeFromKeyboard();

        System.out.println("Матрица:");
        matrix.PrintMatrix();
    }
}
