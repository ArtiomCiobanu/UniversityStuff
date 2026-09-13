package com.android.lab3.ui.quiz;

import com.android.lab3.data.QuizQuestion;

import java.util.List;

public interface QuizPresenter {
    void onQuizQuestionRequested();
    void onAnsweredSubmitted(QuizQuestion quizQuestion, List<String> submittedAnswers);
}
