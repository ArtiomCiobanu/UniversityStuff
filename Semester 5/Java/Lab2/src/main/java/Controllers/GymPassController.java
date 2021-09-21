package Controllers;

import Repositories.GymPassRepository;
import Views.MainView;

public class GymPassController extends BaseController
{
    private GymPassRepository GymPassRepository;

    public GymPassController(
            GymPassRepository gymPassRepository,
            MainView mainView)
    {
        super(mainView);
        
        GymPassRepository = gymPassRepository;
    }
}
