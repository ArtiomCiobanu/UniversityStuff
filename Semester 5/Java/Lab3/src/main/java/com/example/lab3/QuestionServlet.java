package com.example.lab3;

import javax.servlet.annotation.WebServlet;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import java.io.IOException;
import java.io.PrintWriter;

@WebServlet(name = "question", value = "/question-servlet")
public class QuestionServlet extends HttpServlet
{
    //private String message;

    public void init()
    {
        //message = "Hello World!";
    }

    public void doPost(HttpServletRequest request, HttpServletResponse response) throws IOException
    {
        response.setContentType("text/html");

        // Hello
        PrintWriter out = response.getWriter();
        out.println("<html><body>");
        //out.println("<h1>" + message + "</h1>");
        out.println("</body></html>");
    }

    public void destroy()
    {
    }
}
