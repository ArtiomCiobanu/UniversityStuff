package com.company;

public class RectangularTable extends Rectangle
{
    public RectangularTable(double length, double width)
    {
        Set_Length(length);
        Set_Width(width);
    }

    public double GetArea()
    {
        return Get_Length() * Get_Width();
    }

    public double GetPerimeter()
    {
        return (Get_Length() + Get_Width()) * 2;
    }

    public int GetPersonAmount()
    {
        return (int) Math.floor(GetPerimeter() / 0.85);
    }

    public boolean CanFitPeople(int peopleAmount)
    {
        return peopleAmount <= GetPersonAmount();
    }
}
