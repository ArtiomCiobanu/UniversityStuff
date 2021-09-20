import Entities.Manager;
import Mappers.ManagerMapper;
import Repositories.BaseRepository;
import Repositories.ManagerRepository;

import java.sql.SQLException;
import java.util.UUID;

class Lab2
{
    public static void main(String[] args)
    {
        var connectionString = "jdbc:sqlserver://localhost\\SQLEXPRESS;database=Lab2Database;username=Lab2Login;password=Lab2Password";

        var a = new ConnectionFactory();
        var connection = a.getConnection(connectionString);

        var managerRepository = new ManagerRepository(connection);

        //var manager1 = managerRepository.Read(UUID.fromString("EB0A6BCB-78B3-49F3-8843-641AC614F225"));
        //System.out.println(manager1.Name);

        var managers = managerRepository.ReadTop(2);
        for (var manager : managers)
        {
            System.out.println(manager.Name);
        }
        
        try
        {
            connection.close();
        }
        catch (SQLException throwables)
        {
            throwables.printStackTrace();
        }

    }
}
