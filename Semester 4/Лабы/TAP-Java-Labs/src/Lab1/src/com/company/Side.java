package com.company;

public class Side
{
    private double _length;

    public double Get_Length()
    {
        return _length;
    }

    public void Set_Length(double length)
    {
        if (length < 0)
        {
            throw new IllegalArgumentException("Длина должна быть положительной");
        }

        _length = length;
    }
}
