package Controllers;

import Entities.Manager;
import Mappers.SqlMapper;
import Repositories.ManagerRepository;
import Views.EntityInfoTable;

public class ManagerController extends BaseController<Manager>
{
    public ManagerController(
            ManagerRepository managerRepository,
            EntityInfoTable<Manager> managerInfoTable,
            SqlMapper<Manager> managerSqlMapper)
    {
        super(managerRepository, managerInfoTable, managerSqlMapper);
    }
}
