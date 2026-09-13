class Lab1
{
    public static void main(String[] args)
    {
        //fetch student record based on his roll no from the database
        Student model = retrieveStudentFromDatabase();

        //Create a view : to write student details
        StudentView view = new StudentView();

        StudentController controller = new StudentController(view);

        controller.addStudent(new Student("2", "Artiom"));
        controller.addStudent(new Student("3", "Anastasia"));
        controller.setStudentRollNo("Artiom", "1");
    }

    private static Student retrieveStudentFromDatabase()
    {
        Student student = new Student();
        student.setName("Robert");
        student.setRollNo("10");
        return student;
    }
}
