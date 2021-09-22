package Views;

import Entities.BaseEntity;
import Mappers.SqlMapper;

import javax.swing.table.DefaultTableModel;
import java.util.ArrayList;

public class EntityInfoTable<TEntity extends BaseEntity>
{
    private final InfoTable InfoTable;
    private final SqlMapper<TEntity> SqlMapper;

    EntityInfoTable(
            String title,
            String[] columns,
            SqlMapper<TEntity> sqlMapper)
    {
        SqlMapper = sqlMapper;

        InfoTable = new InfoTable(title, columns);
    }

    public void SetTableData(ArrayList<TEntity> tableData)
    {
        var fields = SqlMapper.GetFieldNames();
        String[][] stringTableData = new String[fields.length][tableData.size()];

        int i = 0;
        for (var entity : tableData)
        {
            var entityFields = SqlMapper.GetFields(entity);

            int j = 0;
            for (var field : entityFields)
            {
                stringTableData[i][j] = field.Value;
                j++;
            }

            i++;
        }

        InfoTable.SetTableData(stringTableData);
    }

    public void AddRow(TEntity entity)
    {
        var fields = SqlMapper.GetFields(entity);
        String[] rowData = new String[fields.size()];

        int i = 0;
        for (var field : fields)
        {
            rowData[i] = field.Value;

            i++;
        }

        InfoTable.AddRow(rowData);
    }

    /*public TEntity GetEntityAt(int index)
    {

        var res = InfoTable.GetRowAt(index, column);

        var entity = SqlMapper.

    }*/
}
