package com.android.lab3.view;

import com.android.lab3.data.QuizQuestion;

public interface QuizView {
    void onQuestionGenerated(QuizQuestion quizQuestion);
    void onAnswerCorrect();
    void onAnswerWrong();
    void onError();
}
