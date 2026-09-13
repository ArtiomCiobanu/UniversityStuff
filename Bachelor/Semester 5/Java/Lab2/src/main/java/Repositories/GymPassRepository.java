package Repositories;

import Entities.GymPass;
import Mappers.GymPassMapper;

import java.sql.Connection;

public class GymPassRepository extends BaseRepository<GymPass>
{
    public GymPassRepository(String connectionString)
    {
        super(connectionString, "GymPasses", new GymPassMapper());
    }
}
