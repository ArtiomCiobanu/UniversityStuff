<%@ page import="Repositories.QuestionRepository" %>
<%@ page import="Repositories.AnswerRepository" %>
<%@ page import="Entities.Question" %>
<%@ page import="Entities.Answer" %>
<%@ page import="java.util.*" %>
<%@ page contentType="text/html;charset=UTF-8" language="java" %>

<%
    String connectionString = "jdbc:sqlserver://localhost:1433;username=sa;password=yourStrong(!)Password";

    String[] QuestionNames = new String[]
            {
                    "question1",
                    "question2",
                    "question3",
                    "question4"
            };

    request.setCharacterEncoding("UTF-8");
    response.setCharacterEncoding("UTF-8");
%>

<%
    QuestionRepository questionRepository = new QuestionRepository(connectionString);
    AnswerRepository answerRepository = new AnswerRepository(connectionString);

    ArrayList<Question> questions = questionRepository.ReadTop(QuestionNames.length, 0);
%>

<%
    HashMap<String, String> correctAnswers = new HashMap<>();

    int correctAnswerAmount = 0;
    int totalQuestionAmount = QuestionNames.length;

    for (Question question : questions)
    {
        String currentAnswerText = request.getParameter(question.Name);
        String correctAnswerText = answerRepository.ReadByQuestion(question.Id).Text;

        correctAnswers.put(question.Name, correctAnswerText);

        if (Objects.equals(currentAnswerText, correctAnswerText))
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
<h1>Вы ответили на <%=correctAnswerShare%>% вопросов</h1>
<h2>Правильные ответы:</h2>

<%
    for (String questionName : correctAnswers.keySet())
    {%>
    <b><%=questionName%></b> <br>
    <b>Ваш ответ:</b> <%=request.getParameter(questionName)%><br>
    <b>Правильный ответ:</b> <%=correctAnswers.get(questionName)%><br>
<br>
<% }%>

</body>
</html>
