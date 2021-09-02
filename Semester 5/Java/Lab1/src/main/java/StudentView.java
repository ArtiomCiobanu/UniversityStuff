import javax.swing.JFrame;
import javax.swing.JScrollPane;
import javax.swing.JTable;

public class StudentView
{
    private InfoTable infoTable;

    public void printStudentDetailsInTable(String studentName, String studentRollNo)
    {
        infoTable = new InfoTable(studentName, studentRollNo);
    }
}

class InfoTable
{
    JFrame frame;

    InfoTable(String studentName, String studentRollNo)
    {
        frame = new JFrame();
        frame.setTitle("Student Info");

        String[] columnNames =
                {
                        "Name",
                        "Roll Number"
                };

        String[][] tableData =
                {
                        {studentName, studentRollNo}
                };

        JTable studentInfoTable = new JTable(tableData, columnNames);
        studentInfoTable.setBounds(30, 40, 200, 300);


        JScrollPane scrollPane = new JScrollPane(studentInfoTable);
        frame.add(scrollPane);
        frame.setSize(500, 200);
        frame.setVisible(true);
    }
}