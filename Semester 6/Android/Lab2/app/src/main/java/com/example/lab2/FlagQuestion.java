package com.example.lab2;

public class FlagQuestion {
    public String Flag;
    public String CorrectAnswer;

    public String[] Answers;

    public boolean AnsweredCorrectly;

    public FlagQuestion(String flag, String correctAnswer) {
        Flag = flag;
        CorrectAnswer = correctAnswer;
    }
}
