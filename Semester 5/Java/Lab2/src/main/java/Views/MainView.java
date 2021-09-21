package Views;

import Entities.Client;
import Entities.GymPass;
import Entities.Manager;

public class MainView
{
    public final EntityInfoTable<Client> clientInfoTable;
    public final EntityInfoTable<GymPass> gymPassInfoTable;
    public final EntityInfoTable<Manager> managerInfoTable;

    public MainView()
    {
        this.clientInfoTable = new EntityInfoTable<>(
                "Clients",
                new String[]
                        {
                                "Id",
                                "Name",
                                "RegistrationDate",
                                "GymPassId",
                                "ManagerId"
                        });
        this.gymPassInfoTable = new EntityInfoTable<>(
                "GymPasses",
                new String[]
                        {
                                "Id",
                                "Price",
                                "MonthAmount"
                        });
        this.managerInfoTable = new EntityInfoTable<>(
                "Managers",
                new String[]
                        {
                                "Id",
                                "Name"
                        });

    }
}
