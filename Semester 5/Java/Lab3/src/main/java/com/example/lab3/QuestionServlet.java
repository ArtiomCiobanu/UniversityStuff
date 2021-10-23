package com.example.lab3;

import Repositories.AnswerRepository;
import Repositories.QuestionRepository;

import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Objects;

//docker run -e "ACCEPT_EULA=Y" -e "SA_PASSWORD=yourStrong(!)Password" -e "MSSQL_PID=Express" -p 1433:1433 -d mcr.microsoft.com/mssql/server:2019-latest

@WebServlet(urlPatterns = "/question")
public class QuestionServlet extends HttpServlet
{
    private final QuestionRepository questionRepository;
    private final AnswerRepository answerRepository;

    private final String[] QuestionNames = new String[]
            {
                    "question1",
                    "question2",
                    "question3",
                    "question4"
            };

    public QuestionServlet()
    {
        var connectionString = "jdbc:sqlserver://localhost:1433;username=sa;password=yourStrong(!)Password";

        questionRepository = new QuestionRepository(connectionString);
        answerRepository = new AnswerRepository(connectionString);
    }

    public void init()
    {
    }

    public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException
    {
        request.setCharacterEncoding("UTF-8");
        response.setCharacterEncoding("UTF-8");

        var questions = questionRepository.ReadTop(QuestionNames.length, 0);

        var correctAnswerAmount = 0;
        var totalQuestionAmount = QuestionNames.length;

        for (var question : questions)
        {
            var currentAnswerText = request.getParameter(question.Name);

            var correctAnswer = answerRepository.ReadByQuestion(question.Id);

            if (Objects.equals(currentAnswerText, correctAnswer.Text))
            {
                correctAnswerAmount++;
            }
        }

        var correctAnswerShare = ((double) correctAnswerAmount / totalQuestionAmount) * 100;

        response.setContentType("text/html");

        // Hello
        PrintWriter out = response.getWriter();
        out.println("<html><body>");
        out.println("<h1>" + "Вы ответили на " + correctAnswerShare + "% вопросов" + "</h1>");
        out.println("</body></html>");
    }

    public void destroy()
    {
    }
}
