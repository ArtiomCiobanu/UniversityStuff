package Controllers;

import Entities.Client;
import Mappers.SqlMapper;
import Repositories.ClientRepository;
import Views.EntityInfoTable;

public class ClientController extends BaseController<Client>
{
    public ClientController(
            ClientRepository clientRepository,
            EntityInfoTable<Client> clientTable,
            SqlMapper<Client> entitySqlMapper)
    {
        super(clientRepository, clientTable, entitySqlMapper);
    }

    public void LoadPage(int pageNumber)
    {
        var page = EntityRepository.ReadTop(5, pageNumber * 5);

        EntityTable.SetTableData(page);
    }

    public void RegisterClient(Client client)
    {

    }
}
