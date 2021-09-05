import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.table.DefaultTableModel;

public class StudentView
{
    private final InfoTable infoTable;

    public StudentView()
    {
        infoTable = new InfoTable();
    }

    public void AddStudent(Student student)
    {
        var dataRow = new String[]
                {
                        student.getName(),
                        student.getRollNo()
                };
        infoTable.AddRow(dataRow);
    }

    public void RemoveStudent()
    {

    }

    public void SetStudents(Student[] students)
    {
        var studentTableData = StudentsToStringTableData(students);
        infoTable.SetTableData(studentTableData);
    }

    private String[][] StudentsToStringTableData(Student[] students)
    {
        String[][] result = new String[students.length][2];

        for (int i = 0; i < students.length; i++)
        {
            var currentStudent = students[i];
            result[i][0] = currentStudent.getName();
            result[i][1] = currentStudent.getRollNo();
        }

        return result;
    }

    public void SetStudentRollNo(String studentRollNo, int index)
    {
        var row = infoTable.GetRowAt(index, 1);
        System.out.println(row.length);
    }
}

class InfoTable
{
    private final Object[] columnNames =
            {
                    "Name",
                    "Roll Number"
            };

    private final JFrame frame;
    private JScrollPane scrollPane;
    private JTable studentInfoTable;

    InfoTable()
    {
        frame = new JFrame();
        frame.setTitle("Student Info");

        DefaultTableModel tableModel = new DefaultTableModel();
        tableModel.addColumn("Name");
        tableModel.addColumn("Roll Number");

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