package Views;

import Entities.BaseEntity;
import Mappers.SqlMapper;

import java.awt.event.ActionListener;
import java.util.ArrayList;

public class EntityInfoTable<TEntity extends BaseEntity>
{
    private final InfoTable InfoTable;
    private final SqlMapper<TEntity> SqlMapper;

    public EntityInfoTable(
            String title,
            String[] columns,
            SqlMapper<TEntity> sqlMapper,
            String topButtonLabel,
            String bottomButtonLabel)
    {
        SqlMapper = sqlMapper;

        InfoTable = new InfoTable(
                title,
                columns,
                topButtonLabel,
                bottomButtonLabel);
    }

    public void SetTableData(ArrayList<TEntity> tableData)
    {
        var fields = SqlMapper.GetFieldNames();
        String[][] stringTableData = new String[tableData.size()][fields.length];

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


    public void Show()
    {
        InfoTable.Show();
    }

    public void Hide()
    {
        InfoTable.Hide();
    }

    public void SetTopButtonAction(ActionListener actionListener)
    {
        InfoTable.SetTopButtonAction(actionListener);
    }

    public void SetBottomButtonAction(ActionListener actionListener)
    {
        InfoTable.SetBottomButtonAction(actionListener);
    }
}
