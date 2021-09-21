package Controllers;

import Views.MainView;

public class BaseController
{
    private final MainView MainView;

    public BaseController(MainView mainView)
    {
        MainView = mainView;
    }
}
