package Controllers;

import Entities.GymPass;
import Repositories.GymPassRepository;
import Views.EntityInfoTable;

public class GymPassController extends BaseController<GymPass>
{
    public GymPassController(
            GymPassRepository gymPassRepository,
            EntityInfoTable<GymPass> gymPassInfoTable)
    {
        super(gymPassRepository, gymPassInfoTable);
    }
}
