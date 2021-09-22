package Views;

import javax.swing.*;
import javax.swing.table.DefaultTableModel;
import java.awt.event.ActionListener;

public class InfoTable
{
    private final Object[] columnNames;
    private final JFrame Frame;

    private final JButton TopButton;
    private final JButton BottomButton;

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

        TopButton = new JButton(topButtonLabel);
        TopButton.setSize(100, 20);
        TopButton.setLocation(10, 110);

        BottomButton = new JButton(bottomButtonLabel);
        BottomButton.setSize(100, 20);
        BottomButton.setLocation(10, 140);

        Frame.add(TopButton);
        Frame.add(BottomButton);

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

    public void SetTopButtonAction(ActionListener actionListener)
    {
        TopButton.addActionListener(actionListener);
    }

    public void SetBottomButtonAction(ActionListener actionListener)
    {
        BottomButton.addActionListener(actionListener);
    }
}