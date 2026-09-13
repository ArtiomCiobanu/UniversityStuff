package Mappers;

import Entities.Manager;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
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

    @Override
    public Manager CreateEntity(HashMap<String, String> stringHashMap)
    {
        Manager manager = new Manager();

        manager.Id = UUID.fromString(stringHashMap.get("Id"));
        manager.Name = stringHashMap.get("Name");

        return manager;
    }

    @Override
    public ArrayList<NameValuePair> GetFields(Manager manager)
    {
        ArrayList<NameValuePair> result = new ArrayList<>();

        result.add(new NameValuePair("Id", manager.Id.toString()));
        result.add(new NameValuePair("Name", manager.Name));

        return result;
    }

    @Override
    public String[] GetFieldNames()
    {
        return new String[]
                {
                        "Id",
                        "Name"
                };
    }
}
