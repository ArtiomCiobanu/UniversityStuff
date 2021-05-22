package com.company;


import java.util.Scanner;

//Создайте два подкласса Thread, один, использующий run( ) для запуска,
//и перехватывающий ссылку на второй процесс Thread, а затем вызывающий wait( ).
//Вызов run() второго класса должен вызывать notifyAll( ) для первого процесса
//после нескольких секунд ожидания, так, чтобы первый процесс при этом вывел сообщение.
public class Main
{
    public static void main(String[] args)
    {
        Scanner input = new Scanner(System.in);

        System.out.printf("Число общих процессов: %s%n", Thread.activeCount());

        System.out.println("Введите сообщение: ");
        var message = input.nextLine();

        var notifierThread = new NotifierThread(message);

        var interceptorThread = new InterceptorThread(message, notifierThread);
        interceptorThread.start();
    }
}
