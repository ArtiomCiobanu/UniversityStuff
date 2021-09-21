package Repositories;

import Entities.Client;
import Mappers.ClientMapper;

import java.sql.Connection;

public class ClientRepository extends BaseRepository<Client>
{
    public ClientRepository(Connection connection)
    {
        super(connection, "Clients", new ClientMapper());
    }
}
