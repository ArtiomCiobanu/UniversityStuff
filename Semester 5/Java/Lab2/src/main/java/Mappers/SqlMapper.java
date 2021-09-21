package Mappers;

import Entities.BaseEntity;

import java.sql.ResultSet;
import java.util.ArrayList;

public interface SqlMapper<TEntity extends BaseEntity>
{
    TEntity CreateEntity(ResultSet sql);

    ArrayList<NameValuePair> GetEntityFields(TEntity entity);
}
