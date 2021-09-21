import Controllers.ClientController;
import Controllers.GymPassController;
import Controllers.ManagerController;
import Repositories.ClientRepository;
import Repositories.GymPassRepository;
import Repositories.ManagerRepository;
import Views.MainView;

import java.sql.Connection;
import java.sql.SQLException;

class Lab2
{
    public static void main(String[] args)
    {
        var connectionString = "jdbc:sqlserver://localhost\\SQLEXPRESS;database=Lab2Database;username=Lab2Login;password=Lab2Password";

        var a = new ConnectionFactory();
        var connection = a.getConnection(connectionString);

        Launch(connection);

        try
        {
            connection.close();
        }
        catch (SQLException throwables)
        {
            throwables.printStackTrace();
        }
    }

    private static void Launch(Connection connection)
    {
        var managerRepository = new ManagerRepository(connection);
        var clientRepository = new ClientRepository(connection);
        var gymPassRepository = new GymPassRepository(connection);

        var mainView = new MainView();

        var managerController = new ManagerController(managerRepository, mainView);
        var clientController = new ClientController(clientRepository, mainView);
        var gymPassController = new GymPassController(gymPassRepository, mainView);



        //var manager1 = managerRepository.Read(UUID.fromString("EB0A6BCB-78B3-49F3-8843-641AC614F225"));
        //System.out.println(manager1.Name);

        /*managerRepository.Create(new Manager(UUID.randomUUID(), "Alexandr"));
        var managers = managerRepository.ReadTop(2, 2);
        for (var manager : managers)
        {
            System.out.println(manager.Name);
        }*/
    }
}
