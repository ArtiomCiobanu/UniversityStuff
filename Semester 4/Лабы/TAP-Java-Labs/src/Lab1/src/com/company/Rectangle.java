package com.company;

public class Rectangle extends Side
{
    private double _width;

    public double Get_Width()
    {
        return _width;
    }

    public void Set_Width(double width)
    {
        if (width < 0)
        {
            throw new IllegalArgumentException("Ширина должна быть положительной");
        }

        _width = width;
    }
}
