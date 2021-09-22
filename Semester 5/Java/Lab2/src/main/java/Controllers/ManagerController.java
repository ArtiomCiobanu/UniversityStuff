package Controllers;

import Entities.Manager;
import Repositories.ManagerRepository;
import Views.EntityInfoTable;

public class ManagerController extends BaseController<Manager>
{
    public ManagerController(
            ManagerRepository managerRepository,
            EntityInfoTable<Manager> managerInfoTable)
    {
        super(managerRepository, managerInfoTable);
    }
}
