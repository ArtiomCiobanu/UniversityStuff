package com.android.lab3.storage;

import com.android.lab3.data.QuizQuestion;

import java.util.List;

public interface QuizStorage {
    List<QuizQuestion> loadQuizQuestions();
}
