package Mappers;

import Entities.Manager;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.UUID;

public class ManagerMapper implements SqlMapper<Manager>
{
    @Override
    public Manager CreateEntity(ResultSet sql)
    {
        Manager manager = new Manager();

        try
        {
            manager.Id = UUID.fromString(sql.getString("Id"));
            manager.Name = sql.getString("Name");

            return manager;
        }
        catch (SQLException sqlException)
        {
            sqlException.printStackTrace();
            return null;
        }
    }
}
