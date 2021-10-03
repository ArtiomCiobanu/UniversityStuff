package Controllers;

import Entities.GymPass;
import Mappers.SqlMapper;
import Repositories.GymPassRepository;
import Views.EntityInfoTable;

public class GymPassController extends BaseController<GymPass>
{
    public GymPassController(
            GymPassRepository gymPassRepository,
            EntityInfoTable<GymPass> gymPassInfoTable,
            SqlMapper<GymPass> gymPassSqlMapper)
    {
        super(gymPassRepository, gymPassInfoTable, gymPassSqlMapper);
    }
}
