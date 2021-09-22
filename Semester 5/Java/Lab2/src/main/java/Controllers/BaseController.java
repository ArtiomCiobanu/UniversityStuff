package Controllers;

import Entities.BaseEntity;
import Repositories.Repository;
import Views.EntityInfoTable;

public class BaseController<TEntity extends BaseEntity>
{
    private final EntityInfoTable<TEntity> EntityTable;
    private final Repository<TEntity> EntityRepository;

    public BaseController(
            Repository<TEntity> entityRepository,
            EntityInfoTable<TEntity> entityTable)
    {
        EntityRepository = entityRepository;
        EntityTable = entityTable;
    }

    public void LoadPage(int pageNumber)
    {
        var page = EntityRepository.ReadTop(5, pageNumber * 5);

        EntityTable.SetTableData(page);
    }

    public void Show()
    {
        EntityTable.Show();
    }

    public void Hide()
    {
        EntityTable.Hide();
    }
}
