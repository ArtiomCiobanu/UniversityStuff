package Mappers;

import Entities.BaseEntity;

import java.sql.ResultSet;
import java.util.ArrayList;
import java.util.HashMap;

public interface SqlMapper<TEntity extends BaseEntity>
{
    TEntity CreateEntity(ResultSet sql);
    TEntity CreateEntity(HashMap<String, String> stringHashMap);

    ArrayList<NameValuePair> GetFields(TEntity entity);

    String[] GetFieldNames();
}
