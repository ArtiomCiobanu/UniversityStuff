package Mappers;

import Entities.GymPass;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
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
    public ArrayList<NameValuePair> GetEntityFields(GymPass entity)
    {
        return null;
    }
}
