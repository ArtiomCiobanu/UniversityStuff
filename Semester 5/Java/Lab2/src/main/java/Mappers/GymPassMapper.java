package Mappers;

import Entities.GymPass;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.UUID;

public class GymPassMapper implements SqlMapper<GymPass>
{
    @Override
    public GymPass CreateEntity(ResultSet sql)
    {
        GymPass gymPass = new GymPass();

        try
        {
            gymPass.Id = UUID.fromString(sql.getString("Id"));
            gymPass.Price = sql.getInt("Price");
            gymPass.MonthAmount = sql.getInt("MonthAmount");
        }
        catch (SQLException sqlException)
        {
            sqlException.printStackTrace();
            return null;
        }

        return gymPass;
    }

    @Override
    public GymPass CreateEntity(HashMap<String, String> stringHashMap)
    {
        GymPass gymPass = new GymPass();

        gymPass.Id = UUID.fromString(stringHashMap.get("Id"));
        gymPass.MonthAmount = Integer.parseInt(stringHashMap.get("MonthAmount"));
        gymPass.Price = Integer.parseInt(stringHashMap.get("Price"));

        return gymPass;
    }

    @Override
    public ArrayList<NameValuePair> GetFields(GymPass gymPass)
    {
        ArrayList<NameValuePair> result = new ArrayList<>();

        result.add(new NameValuePair("Id", gymPass.Id.toString()));
        result.add(new NameValuePair("MonthAmount", Integer.toString(gymPass.MonthAmount)));
        result.add(new NameValuePair("Price", Integer.toString(gymPass.Price)));

        return result;
    }

    @Override
    public String[] GetFieldNames()
    {
        return new String[]
                {
                        "Id",
                        "MonthAmount",
                        "Price"
                };
    }
}
