package Controllers;

import Repositories.ManagerRepository;
import Views.MainView;

public class ManagerController extends BaseController
{
    private final ManagerRepository ManagerRepository;

    public ManagerController(
            ManagerRepository managerRepository,
            MainView mainView)
    {
        super(mainView);

        ManagerRepository = managerRepository;
    }
}
