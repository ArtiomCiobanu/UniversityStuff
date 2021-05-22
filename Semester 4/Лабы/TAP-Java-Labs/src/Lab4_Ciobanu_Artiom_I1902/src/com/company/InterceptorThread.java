package com.company;

public class InterceptorThread extends Thread
{
    private final String _monitor;
    private Thread _runnable;

    public InterceptorThread(String monitor, NotifierThread notifierThread)
    {
        _monitor = monitor;
        _runnable = notifierThread;
    }

    @Override
    public void run()
    {
        System.out.println("InterceptorThread has started to work!");

        _runnable.start();

        try
        {
            System.out.println("InterceptorThread is waiting.");

            synchronized (_monitor)
            {
                _monitor.wait();
            }

            System.out.println("InterceptorThread is resuming.");

            System.out.println(_monitor);
        } catch (InterruptedException interruptedException)
        {
            //interrupt();
            System.out.println(interruptedException.getMessage());
        }

    }
}
