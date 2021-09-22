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

        var managerController = new ManagerController(managerRepository, mainView.managerInfoTable);
        var clientController = new ClientController(clientRepository, mainView.clientInfoTable);
        var gymPassController = new GymPassController(gymPassRepository, mainView.gymPassInfoTable);

        managerController.LoadPage(0);
        managerController.Show();
    }
}
