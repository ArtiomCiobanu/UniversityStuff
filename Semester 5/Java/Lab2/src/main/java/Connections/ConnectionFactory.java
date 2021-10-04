package Connections;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class ConnectionFactory
{
    Connection connection = null;

    public Connection getConnection(String url)
    {
        try
        {
            connection = DriverManager.getConnection(url);
        } catch (SQLException throwables)
        {
            throwables.printStackTrace();
        }

        return connection;
    }
}
