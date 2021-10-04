package Repositories;

import Entities.Manager;
import Mappers.ManagerMapper;

import java.sql.Connection;

public class ManagerRepository extends BaseRepository<Manager>
{
    public ManagerRepository(String connectionString)
    {
        super(connectionString, "Managers", new ManagerMapper());
    }
}
