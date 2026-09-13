package Repositories;

import Connections.ConnectionFactory;
import Entities.BaseEntity;
import Mappers.SqlMapper;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.UUID;

public class BaseRepository<TEntity extends BaseEntity> implements Repository<TEntity>
{
    private final String tableName;
    private final SqlMapper<TEntity> sqlMapper;

    private final String connectionString;

    protected final ConnectionFactory ConnectionFactory = new ConnectionFactory();

    //Should be disposed after every query
    private Connection connection;

    public BaseRepository(
            String connectionString,
            String tableName,
            SqlMapper<TEntity> sqlMapper)
    {
        this.connectionString = connectionString;
        this.sqlMapper = sqlMapper;

        this.tableName = tableName;
    }


    public void Create(TEntity entity)
    {
        StringBuilder sqlQuery = new StringBuilder(String.format("insert into %s values(", tableName));

        var fields = sqlMapper.GetFields(entity);

        for (var field : fields)
        {
            sqlQuery.append(String.format("'%s',", field.Value));
        }

        sqlQuery.deleteCharAt(sqlQuery.length() - 1);
        sqlQuery.append(")");

        Execute(sqlQuery.toString());

        CloseConnection();
    }

    public TEntity Read(UUID id)
    {
        var sqlQuery = String.format("select * from %s where Id = '%s'", tableName, id);

        var queryResult = ExecuteQuery(sqlQuery);

        var entity = queryResult != null ? GetNextEntity(queryResult) : null;

        CloseConnection();

        return entity;
    }

    public ArrayList<TEntity> ReadTop(int amount, int offset)
    {
        var sqlQuery = String.format("select * from %s order by Id offset %s rows fetch next %s rows only", tableName, offset, amount);

        var queryResult = ExecuteQuery(sqlQuery);
        if (queryResult == null)
        {
            return null;
        }

        var result = new ArrayList<TEntity>();
        for (int i = 0; i < amount; i++)
        {
            var entity = GetNextEntity(queryResult);

            if (entity == null)
            {
                break;
            }

            result.add(entity);
        }

        CloseConnection();

        return result;
    }

    public void Update(TEntity entity)
    {
        if (ExistsWithId(entity.Id))
        {
            StringBuilder sqlQuery = new StringBuilder(String.format("update %s set ", tableName));

            var fields = sqlMapper.GetFields(entity);

            for (var field : fields)
            {
                sqlQuery.append(String.format("set %s = %s,", field.Name, field.Value));
            }
            sqlQuery.deleteCharAt(sqlQuery.length() - 1);

            sqlQuery.append(String.format("where Id = %s", entity.Id));
        }

        CloseConnection();
    }

    public void Delete(UUID id)
    {
        var sqlQuery = String.format("delete from %s where Id = \'%s\'", tableName, id);

        Execute(sqlQuery);

        CloseConnection();
    }

    public boolean ExistsWithId(UUID id)
    {
        var sqlQuery = String.format("if exists (select * from Managers where id = '%s')", id) +
                "select 1 as Exists else select 0 as Exists";

        var resultSet = ExecuteQuery(sqlQuery);
        try
        {
            if (!resultSet.next())
            {
                return false;
            }

            var exists = resultSet.getBoolean("Exists");

            CloseConnection();

            return exists;
        }
        catch (SQLException ex)
        {
            ex.printStackTrace();

            CloseConnection();

            return false;
        }
    }


    private TEntity GetNextEntity(ResultSet resultSet)
    {
        try
        {
            var exists = resultSet.next();

            return exists ? sqlMapper.CreateEntity(resultSet) : null;
        }
        catch (SQLException ex)
        {
            ex.printStackTrace();

            return null;
        }
    }

    private ResultSet ExecuteQuery(String sqlQuery)
    {
        System.out.println(sqlQuery);

        try
        {
            var connection = ConnectionFactory.getConnection(connectionString);
            var statement = connection.prepareStatement(sqlQuery);

            return statement.executeQuery();
        }
        catch (SQLException ex)
        {
            ex.printStackTrace();
            return null;
        }
    }

    private void Execute(String sqlQuery)
    {
        System.out.println(sqlQuery);

        try
        {
            var connection = ConnectionFactory.getConnection(connectionString);
            var statement = connection.prepareStatement(sqlQuery);

            statement.execute();

            connection.close();
        }
        catch (SQLException ex)
        {
            ex.printStackTrace();
        }
    }

    private void CloseConnection()
    {
        try
        {
            if(connection != null && !connection.isClosed())
            {
                connection.close();
            }
        }
        catch (SQLException e)
        {
            e.printStackTrace();
        }
    }
}
