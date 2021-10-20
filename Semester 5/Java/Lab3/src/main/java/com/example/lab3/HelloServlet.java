package com.example.lab3;

import java.io.*;
import javax.servlet.ServletConfig;
import javax.servlet.ServletException;
import javax.servlet.http.*;
import javax.servlet.annotation.*;

/*
Лабораторная работа 3. Программирование сервлетов (Servlet)

Разработать страницу html для тестирования знаний или чего-либо ещё в какой-либо области. Например, география, геометрия, иностранный язык, анатомия, и т.д. Тему можно выбрать самостоятельно и прислать мне на одобрение.
Необходимо использовать все элементы формы (html-form). Используйте также элемент map (map с примером), для генерации кода можно использовать map генератор (http://htmlmapgenerator.ru/), данные от map можно хранить в hidden поле, а записывать данные в это поле можно при помощи маленькой JavaScript функции.

Данные из формы отправляются сервлету для обработки. Сервлет обрабатывает данные, формирует и отправляет страницу с результатами теста клиенту (браузеру).
Вопросы и ответы хранить в базе данных. Это требование не касается п1.
Nℬ. В IDE надо установит сервер, работающий с Java (TomEE/Tomcat). Также должна быть установлена Java EE (Java Enterprise Edition).

Тема: Тестирование знаний по астрономии (космос, планеты, солнечная система).
 */

//@WebServlet(name = "helloServlet", value = "/hello")
@WebServlet(urlPatterns = "/hello")
public class HelloServlet extends HttpServlet
{
    public HelloServlet()
    {

    }

    public void init(ServletConfig servletConfig)
    {
        try {
            super.init(servletConfig);
        } catch (ServletException servletException) {
            servletException.printStackTrace();
        }
    }

    public void doGet(HttpServletRequest request, HttpServletResponse response) throws IOException
    {
        response.setContentType("text/html");

        // Hello
        PrintWriter out = response.getWriter();
        out.println("<html><body>");
        out.println("<h1>" + "message" + "</h1>");
        out.println("</body></html>");
    }

    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException
    {

    }

    public void destroy()
    {
    }
}