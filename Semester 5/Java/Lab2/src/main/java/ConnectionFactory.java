import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class ConnectionFactory
{
    Connection connection = null;

    public Connection getConnection(String url/*, String username, String password*/)
    {
        System.out.println("Connecting database...");

        try
        {
            connection = DriverManager.getConnection(url);
        } catch (SQLException throwables)
        {
            throwables.printStackTrace();
        }

        System.out.println("Database connected!");

        return connection;
    }
}
