package com.company;

public class NotifierThread extends Thread
{
    private final String _monitor;

    public NotifierThread(String monitor)
    {
        _monitor = monitor;
    }

    @Override
    public void run()
    {
        System.out.println("NotifierThread has started to work!");

        try
        {
            synchronized (_monitor)
            {
                _monitor.wait(1000);

                System.out.println("NotifierThread is notifying.");
                _monitor.notifyAll();
            }
        } catch (InterruptedException interruptedException)
        {
            //interrupt();
            System.out.println(interruptedException.getMessage());
        }
    }
}
