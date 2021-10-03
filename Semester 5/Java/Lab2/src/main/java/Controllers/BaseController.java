package Controllers;

import Entities.BaseEntity;
import Mappers.SqlMapper;
import Repositories.Repository;
import Views.AddEntityView;
import Views.EntityInfoTable;
import Views.RemoveEntityView;

public class BaseController<TEntity extends BaseEntity>
{
    protected final EntityInfoTable<TEntity> EntityTable;
    protected final Repository<TEntity> EntityRepository;
    protected final SqlMapper<TEntity> EntitySqlMapper;

    public BaseController(
            Repository<TEntity> entityRepository,
            EntityInfoTable<TEntity> entityTable,
            SqlMapper<TEntity> entitySqlMapper)
    {
        EntityRepository = entityRepository;
        EntityTable = entityTable;
        EntitySqlMapper = entitySqlMapper;
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

    public void Add()
    {
        var addEntityView = new AddEntityView<TEntity>(EntitySqlMapper);

        var entity = addEntityView.Result;

        EntityRepository.Create(entity);
    }

    public void Remove()
    {
        var removeEntityView = new RemoveEntityView();

        var id = removeEntityView.EntityId;

        EntityRepository.Delete(id);
    }
}
