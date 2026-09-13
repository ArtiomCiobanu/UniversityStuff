package com.company;

public class FakeFloat
{
    private double _value;

    public double Get_Value()
    {
        return _value;
    }

    public void Set_Value(double value) throws NotAllowedNumberException
    {
        if (value == Double.NEGATIVE_INFINITY || Double.isNaN(value))
        {
            _value = 33333;
        }

        if (value > 35767 || value < -35768 ||
                value == Double.POSITIVE_INFINITY)
        {
            _value = 3333;
            throw new NotAllowedNumberException("Значение должно быть больше чем 35767 и меньше -35768");
        }

        _value = value;
    }

    public FakeFloat(double value)
    {
        _value = value;
    }

    public void AddFakeFloat(FakeFloat value) throws NotAllowedNumberException
    {
        Set_Value(_value + value.Get_Value());
    }

    public void SubstractFakeFloat(FakeFloat value) throws NotAllowedNumberException
    {
        Set_Value(_value - value.Get_Value());
    }

    public void DivideByFakeFloat(FakeFloat value) throws NotAllowedNumberException
    {
        Set_Value(_value / value.Get_Value());
    }

    public void MultiplyByFakeFloat(FakeFloat value) throws NotAllowedNumberException
    {
        Set_Value(_value * value.Get_Value());
    }

    //Остаток от деления
    public void Remainder(FakeFloat value) throws NotAllowedNumberException
    {
        Set_Value(_value % value.Get_Value());
    }
}
