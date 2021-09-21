package Repositories;

import Entities.GymPass;
import Mappers.GymPassMapper;

import java.sql.Connection;

public class GymPassRepository extends BaseRepository<GymPass>
{
    public GymPassRepository(Connection connection)
    {
        super(connection, "GymPasses", new GymPassMapper());
    }
}
