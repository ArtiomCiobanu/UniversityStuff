package Mappers;

import java.sql.ResultSet;

public interface SqlMapper<TEntity>
{
    TEntity CreateEntity(ResultSet sql);
}
