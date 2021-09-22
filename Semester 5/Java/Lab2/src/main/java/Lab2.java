import Controllers.ClientController;
import Controllers.GymPassController;
import Controllers.ManagerController;
import Mappers.ClientMapper;
import Mappers.GymPassMapper;
import Mappers.ManagerMapper;
import Repositories.ClientRepository;
import Repositories.GymPassRepository;
import Repositories.ManagerRepository;
import Views.EntityInfoTable;

import java.sql.Connection;
import java.sql.SQLException;

class Lab2
{
    private static ClientController clientController;
    private static GymPassController gymPassController;
    private static ManagerController managerController;

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

        var clientInfoTable = new EntityInfoTable<>(
                "Clients",
                new String[]
                        {
                                "Id",
                                "Name",
                                "RegistrationDate",
                                "GymPassId",
                                "ManagerId"
                        },
                new ClientMapper(),
                "Gym Passes",
                "Managers");
        var gymPassInfoTable = new EntityInfoTable<>(
                "GymPasses",
                new String[]
                        {
                                "Id",
                                "Price",
                                "MonthAmount"
                        },
                new GymPassMapper(),
                "Managers",
                "Clients");
        var managerInfoTable = new EntityInfoTable<>(
                "Managers",
                new String[]
                        {
                                "Id",
                                "Name"
                        },
                new ManagerMapper(),
                "Clients",
                "GymPasses");

        clientInfoTable.SetTopButtonAction(e -> GymPassesButton_Click());
        clientInfoTable.SetBottomButtonAction(e -> ManagersButton_Click());

        gymPassInfoTable.SetTopButtonAction(e -> ManagersButton_Click());
        gymPassInfoTable.SetBottomButtonAction(e -> ClientsButton_Click());

        managerInfoTable.SetTopButtonAction(e -> ClientsButton_Click());
        managerInfoTable.SetBottomButtonAction(e -> GymPassesButton_Click());

        managerController = new ManagerController(managerRepository, managerInfoTable);
        clientController = new ClientController(clientRepository, clientInfoTable);
        gymPassController = new GymPassController(gymPassRepository, gymPassInfoTable);

        managerController.LoadPage(0);
        managerController.Show();
    }

    static void GymPassesButton_Click()
    {
        managerController.Hide();
        clientController.Hide();

        gymPassController.Show();
    }

    static void ClientsButton_Click()
    {
        managerController.Hide();
        gymPassController.Hide();

        clientController.Show();
    }

    static void ManagersButton_Click()
    {
        gymPassController.Hide();
        clientController.Hide();

        managerController.Show();
    }
}
