package Entities;

import java.util.UUID;

public class Manager extends BaseEntity
{
    public String Name;

    public Manager()
    {

    }

    public Manager(UUID id, String name)
    {
        this.Id = id;
        this.Name = name;
    }
}
