package Repositories;

import Mappers.SqlMapper;

import java.sql.Array;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.UUID;

public class BaseRepository<TEntity>
{
    private final String tableName;
    private final SqlMapper<TEntity> sqlMapper;

    protected final Connection connection;

    public BaseRepository(
            Connection connection,
            String tableName,
            SqlMapper<TEntity> sqlMapper)
    {
        this.connection = connection;
        this.sqlMapper = sqlMapper;

        this.tableName = tableName;
    }

    public TEntity Read(UUID id)
    {
        var sqlQuery = String.format("select * from %s where Id = '%s'", tableName, id);

        var queryResult = ExecuteQuery(sqlQuery);

        var entity = queryResult != null ? GetNextEntity(queryResult) : null;
        return entity;
    }

    public ArrayList<TEntity> ReadTop(int amount)
    {
        var sqlQuery = String.format("select top %s * from %s", amount, tableName);

        var queryResult = ExecuteQuery(sqlQuery);
        if (queryResult == null)
        {
            return null;
        }

        //var result = (TEntity[]) new ArrayList<TEntity>(amount).toArray();
        //var result = (TEntity[]) Array.newInstance(new Class<TEntity>(), amount);

        //var result = new Object[amount];


        var result = new ArrayList<TEntity>();
        for (int i = 0; i < amount; i++)
        {
            var entity = GetNextEntity(queryResult);
            //result[i] = entity;
            result.add(entity);
        }

        return result;
    }

    private TEntity GetNextEntity(ResultSet resultSet)
    {
        try
        {
            resultSet.next();

            var entity = sqlMapper.CreateEntity(resultSet);
            return entity;
        }
        catch (SQLException ex)
        {
            ex.printStackTrace();
            return null;
        }
    }

    private ResultSet ExecuteQuery(String sqlQuery)
    {
        try
        {
            var statement = connection.prepareStatement(sqlQuery);

            var queryResult = statement.executeQuery();
            return queryResult;
        }
        catch (SQLException ex)
        {
            ex.printStackTrace();
            return null;
        }
    }
}
