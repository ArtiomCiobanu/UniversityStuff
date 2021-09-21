package Views;

import Entities.Client;
import Entities.GymPass;
import Entities.Manager;

public class MainView
{
    private final InfoTable<Client> clientInfoTable;
    private final InfoTable<GymPass> gymPassInfoTable;
    private final InfoTable<Manager> managerInfoTable;

    public MainView()
    {
        this.clientInfoTable = new InfoTable<>(
                "Clients",
                new String[]
                        {
                                "Id",
                                "Name",
                                "RegistrationDate",
                                "GymPassId",
                                "ManagerId"
                        });
        this.gymPassInfoTable = new InfoTable<>(
                "GymPasses",
                new String[]
                        {
                                "Id",
                                "Price",
                                "MonthAmount"
                        });
        this.managerInfoTable = new InfoTable<>(
                "Managers",
                new String[]
                        {
                                "Id",
                                "Name"
                        });

    }
}
