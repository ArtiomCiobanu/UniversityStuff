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
    public ArrayList<NameValuePair> GetFields(Client client)
    {
        ArrayList<NameValuePair> result = new ArrayList<>();

        result.add(new NameValuePair("Id", client.Id.toString()));
        result.add(new NameValuePair("Name", client.Name));
        result.add(new NameValuePair("RegistrationDate", client.RegistrationDate.toString()));

        result.add(new NameValuePair("ManagerId", client.ManagerId.toString()));
        result.add(new NameValuePair("GymPassId", client.GymPassId.toString()));

        return result;
    }

    @Override
    public String[] GetFieldNames()
    {
        return new String[]
                {
                        "Id",
                        "Name",
                        "RegistrationDate",
                        "ManagerId",
                        "GymPassId"
                };
    }
}
