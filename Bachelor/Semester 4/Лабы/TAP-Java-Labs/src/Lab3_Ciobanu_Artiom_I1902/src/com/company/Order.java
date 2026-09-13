package com.company;

public class Order
{
    public int Number;
    public int CreationYear;
    public int HouseNumber;
    public String FirstName;
    public String LastName;
    public String Description;

    public Order()
    {

    }

    public Order(int number, int creationYear, int houseNumber, String firstName, String lastName, String description)
    {
        Number = number;
        CreationYear = creationYear;
        HouseNumber = houseNumber;
        FirstName = firstName;
        LastName = lastName;
        Description = description;
    }

    @Override
    public String toString()
    {
        return String.format("%s %s %s %s %s %s %n", Number, CreationYear, HouseNumber, FirstName, LastName, Description);
    }
}
