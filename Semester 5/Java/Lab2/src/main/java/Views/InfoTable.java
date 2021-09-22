package Views;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;

public class InfoTable
{
    private final Object[] columnNames;

    private final JFrame Frame;
    private JScrollPane ScrollPane;
    private JTable StudentInfoTable;

    InfoTable(
            String title,
            String[] columns,
            String topButtonLabel,
            String bottomButtonLabel)
    {
        Frame = new JFrame();
        Frame.setTitle(title);

        var topButton = new JButton(topButtonLabel);
        topButton.setSize(100, 20);
        topButton.setLocation(10, 110);

        var bottomButton = new JButton(bottomButtonLabel);
        bottomButton.setSize(100, 20);
        bottomButton.setLocation(10, 140);

        Frame.add(topButton);
        Frame.add(bottomButton);


        DefaultTableModel tableModel = new DefaultTableModel();

        for (var column : columns)
        {
            tableModel.addColumn(column);
        }

        columnNames = columns;

        StudentInfoTable = new JTable(tableModel);
        StudentInfoTable.setBounds(30, 40, 200, 300);

        ScrollPane = new JScrollPane(StudentInfoTable);
        Frame.add(ScrollPane);
        Frame.setSize(500, 400);
    }

    public void SetTableData(String[][] tableData)
    {
        StudentInfoTable = new JTable(tableData, columnNames);
        StudentInfoTable.setBounds(30, 40, 200, 300);

        Frame.remove(ScrollPane);
        ScrollPane = new JScrollPane(StudentInfoTable);
        Frame.add(ScrollPane);

        Frame.update(Frame.getGraphics());
    }

    public void AddRow(String[] row)
    {
        var model = (DefaultTableModel) StudentInfoTable.getModel();

        var rowAmount = model.getDataVector().size();
        model.addRow(row);
        model.fireTableRowsUpdated(rowAmount - 2, rowAmount - 1);
    }

    public String[] GetRowAt(int index, int column)
    {
        var model = (DefaultTableModel) StudentInfoTable.getModel();
        var result = model.getValueAt(index, column);

        return (String[]) result;
    }

    public void Show()
    {
        Frame.setVisible(true);
    }

    public void Hide()
    {
        Frame.setVisible(false);
    }
}