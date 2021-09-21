package Controllers;

import Repositories.ClientRepository;
import Views.MainView;

public class ClientController extends BaseController
{
    private final ClientRepository ClientRepository;

    public ClientController(
            ClientRepository clientRepository,
            MainView mainView)
    {
        super(mainView);

        ClientRepository = clientRepository;
    }

    public void LoadPage(int pageNumber)
    {
        var page = ClientRepository.ReadTop(5, pageNumber * 5);
    }

    public void RegisterClient()
    {

    }
}
