package Mappers;

import Entities.Client;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.UUID;

public class ClientMapper implements SqlMapper<Client>
{
    @Override
    public Client CreateEntity(ResultSet sql)
    {
        Client client = new Client();

        try
        {
            client.Id = UUID.fromString(sql.getString("Id"));
            client.GymPassId = UUID.fromString(sql.getString("GymPassId"));
            client.ManagerId = UUID.fromString(sql.getString("ManagerId"));

            client.Name = sql.getString("Name");
            client.RegistrationDate = sql.getDate("RegistrationDate");
        }
        catch (SQLException sqlException)
        {
            sqlException.printStackTrace();
            return null;
        }

        return client;
    }

    @Override
    public ArrayList<NameValuePair> GetEntityFields(Client entity)
    {
        ArrayList<NameValuePair> result = new ArrayList<>();

        result.add(new NameValuePair("Id", entity.Id.toString()));
        result.add(new NameValuePair("Name", entity.Name));

        return result;
    }
}
