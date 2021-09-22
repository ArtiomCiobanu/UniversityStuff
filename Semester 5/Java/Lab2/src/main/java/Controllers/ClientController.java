package Controllers;

import Entities.Client;
import Repositories.ClientRepository;
import Views.EntityInfoTable;

public class ClientController
{
    private final ClientRepository ClientRepository;
    private final EntityInfoTable<Client> ClientTable;

    public ClientController(
            ClientRepository clientRepository,
            EntityInfoTable<Client> clientTable)
    {
        ClientRepository = clientRepository;
        ClientTable = clientTable;
    }

    public void LoadPage(int pageNumber)
    {
        var page = ClientRepository.ReadTop(5, pageNumber * 5);

        ClientTable.SetTableData(page);
    }

    public void RegisterClient(Client client)
    {

    }
}
