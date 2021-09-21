package Controllers;

import Entities.Client;
import Repositories.ClientRepository;
import Views.EntityInfoTable;

public class ClientController
{
    private final ClientRepository ClientRepository;

    public ClientController(
            ClientRepository clientRepository,
            EntityInfoTable<Client> mainView)
    {

        ClientRepository = clientRepository;
    }

    public void LoadPage(int pageNumber)
    {
        var page = ClientRepository.ReadTop(5, pageNumber * 5);

    }

    public void RegisterClient(Client client)
    {

    }
}
