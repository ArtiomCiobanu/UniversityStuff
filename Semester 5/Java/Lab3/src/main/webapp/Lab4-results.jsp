<%@ page import="Repositories.QuestionRepository" %>
<%@ page import="Repositories.AnswerRepository" %>
<%@ page import="java.util.Objects" %>
<%@ page import="java.util.ArrayList" %>
<%@ page import="Entities.Question" %>
<%@ page import="Entities.Answer" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<%
    request.setCharacterEncoding("UTF-8");
    response.setCharacterEncoding("UTF-8");

    String connectionString = "jdbc:sqlserver://localhost:1433;username=sa;password=yourStrong(!)Password";
    QuestionRepository questionRepository = new QuestionRepository(connectionString);
    AnswerRepository answerRepository = new AnswerRepository(connectionString);

    String[] QuestionNames = new String[]
            {
                    "question1",
                    "question2",
                    "question3",
                    "question4"
            };

    ArrayList<Question> questions = questionRepository.ReadTop(QuestionNames.length, 0);

    int correctAnswerAmount = 0;
    int totalQuestionAmount = QuestionNames.length;

    for (Question question : questions)
    {
        String currentAnswerText = request.getParameter(question.Name);

        Answer correctAnswer = answerRepository.ReadByQuestion(question.Id);

        if (Objects.equals(currentAnswerText, correctAnswer.Text))
        {
            correctAnswerAmount++;
        }
    }

    double correctAnswerShare = ((double) correctAnswerAmount / totalQuestionAmount) * 100;
%>

<html>
<head>
    <title>Title</title>
</head>
<body>
<h1>Вы ответили на <%=correctAnswerShare%>% вопросов
</h1>
</body>
</html>
