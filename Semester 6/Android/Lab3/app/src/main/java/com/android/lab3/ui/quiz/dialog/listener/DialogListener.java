package com.android.lab3.ui.quiz.dialog.listener;

import com.android.lab3.data.QuizQuestion;

import java.util.List;

public interface DialogListener {
    void onNewQuestionRequested();
    void onAnswerSubmitted(QuizQuestion quizQuestion, List<String> submittedAnswers);
}
