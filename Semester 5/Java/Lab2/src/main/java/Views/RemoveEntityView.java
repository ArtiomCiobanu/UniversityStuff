package Views;

import Entities.BaseEntity;
import Mappers.SqlMapper;

import javax.swing.*;

public class RemoveEntityView<TEntity extends BaseEntity>
{
    private final JFrame Frame;

    public RemoveEntityView(SqlMapper<TEntity> entitySqlMapper)
    {
        Frame = new JFrame();
        Frame.setTitle("Add Entity");

        Frame.setVisible(true);
    }
}
