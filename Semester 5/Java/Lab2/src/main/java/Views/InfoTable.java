package Views;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;

public class InfoTable
{
    private final Object[] columnNames =
            {
                    "Name",
                    "Roll Number"
            };

    private final JFrame frame;
    private JScrollPane scrollPane;
    private JTable studentInfoTable;

    InfoTable(String title, String[] columns)
    {
        frame = new JFrame();
        frame.setTitle(title);

        DefaultTableModel tableModel = new DefaultTableModel();

        for (var column : columns)
        {
            tableModel.addColumn(column);
        }

        studentInfoTable = new JTable(tableModel);
        studentInfoTable.setBounds(30, 40, 200, 300);

        JScrollPane scrollPane = new JScrollPane(studentInfoTable);
        frame.add(scrollPane);
        frame.setSize(500, 200);
        frame.setVisible(true);
    }

    public void SetTableData(String[][] tableData)
    {
        studentInfoTable = new JTable(tableData, columnNames);
        studentInfoTable.setBounds(30, 40, 200, 300);

        frame.remove(scrollPane);
        scrollPane = new JScrollPane(studentInfoTable);
        frame.add(scrollPane);

        frame.update(frame.getGraphics());
    }

    public void AddRow(String[] row)
    {
        var model = (DefaultTableModel) studentInfoTable.getModel();

        var rowAmount = model.getDataVector().size();
        model.addRow(row);
        model.fireTableRowsUpdated(rowAmount - 2, rowAmount - 1);
    }

    public String[] GetRowAt(int index, int column)
    {
        var model = (DefaultTableModel) studentInfoTable.getModel();
        var result = model.getValueAt(index, column);

        return (String[]) result;
    }
}