package Controllers;

import Views.MainView;

public class BaseController
{
    protected final MainView MainView;

    public BaseController(MainView mainView)
    {
        MainView = mainView;
    }
}
