package Views;

import java.util.ArrayList;

public class EntityInfoTable<TEntity>
{
    private InfoTable InfoTable;

    EntityInfoTable(String title, String[] columns)
    {
        InfoTable = new InfoTable(title, columns);
    }

    public void SetTableData(ArrayList<TEntity> tableData)
    {

    }

    public void AddRow(TEntity entity)
    {

    }

    public String GetRowAt(int index)
    {

    }
}
