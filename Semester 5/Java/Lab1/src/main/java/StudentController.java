import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;

public class StudentController
{
    private final StudentView view;

    private final List<Student> students;

    public StudentController(Student[] defaultStudents)
    {
        this.students = Arrays.asList(defaultStudents);

        view = null;
    }

    public StudentController(StudentView view)
    {
        this.view = view;

        students = new ArrayList<>();
    }

    public void setStudentRollNo(String studentName, String rollNo)
    {
        students.stream()
                .filter(s -> Objects.equals(s.getName(), studentName))
                .findFirst()
                .ifPresent(student ->
                {
                    student.setRollNo(rollNo);
                    view.SetStudentRollNo(rollNo, students.indexOf(student));
                });
    }

    public String getStudentRollNo(String studentName)
    {
        var student = students.stream()
                .filter(s -> Objects.equals(s.getName(), studentName))
                .findFirst()
                .orElse(null);

        return student != null
                ? student.getRollNo()
                : null;
    }


    public void addStudent(Student student)
    {
        if (student != null)
        {
            view.AddStudent(student);
        }
    }

    public void setStudents(Student[] students)
    {
        if (students != null && students.length > 0)
        {
            view.SetStudents(students);
        }
    }
}