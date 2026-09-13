package com.android.lab3.data;

import java.util.List;

public class QuizQuestion {
    private String title;
    private QuestionType type;
    private List<String> possibleAnswers;
    private List<String> correctAnswers;

    public QuizQuestion(String title, QuestionType type, List<String> possibleAnswers, List<String> correctAnswers) {
        this.title = title;
        this.type = type;
        this.possibleAnswers = possibleAnswers;
        this.correctAnswers = correctAnswers;
    }

    public String getTitle() {
        return title;
    }

    public QuestionType getType() {
        return type;
    }

    public List<String> getPossibleAnswers() {
        return possibleAnswers;
    }

    public List<String> getCorrectAnswers() {
        return correctAnswers;
    }
}
