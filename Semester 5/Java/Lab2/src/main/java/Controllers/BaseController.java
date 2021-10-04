package Controllers;

import Entities.BaseEntity;
import Mappers.SqlMapper;
import Repositories.Repository;
import Views.AddEntityView;
import Views.EntityInfoTable;
import Views.RemoveEntityView;

import javax.swing.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class BaseController<TEntity extends BaseEntity>
{
    protected final EntityInfoTable<TEntity> EntityTable;
    protected final Repository<TEntity> EntityRepository;
    protected final SqlMapper<TEntity> EntitySqlMapper;

    private int currentPage;

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
        currentPage = pageNumber;

        var page = EntityRepository.ReadTop(5, currentPage * 5);

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
        var addEntityView = new AddEntityView<>(EntitySqlMapper);

        addEntityView.SetExitAction(() ->
        {
            var entity = addEntityView.Result;

            EntityRepository.Create(entity);

            LoadPage(currentPage);
        });
    }

    public void Remove()
    {
        var removeEntityView = new RemoveEntityView();
        removeEntityView.SetExitAction(() ->
        {
            var id = removeEntityView.EntityId;

            EntityRepository.Delete(id);

            LoadPage(currentPage);
        });
    }
}
