package com.android.lab3.storage;

import android.content.res.Resources;

import androidx.annotation.ArrayRes;
import androidx.annotation.StringRes;

import com.android.lab3.R;
import com.android.lab3.data.QuestionType;
import com.android.lab3.data.QuizQuestion;

import java.util.Arrays;
import java.util.List;

public class DefaultQuizStorage implements QuizStorage {
    private final Resources resources;

    public DefaultQuizStorage(Resources resources) {
        this.resources = resources;
    }

    @Override
    public List<QuizQuestion> loadQuizQuestions() {
        return Arrays.asList(
                getFromResource(R.string.title_question_1, QuestionType.SINGLE_CHOICE, R.array.answers_question_1, R.array.solution_question_1),
                getFromResource(R.string.title_question_2, QuestionType.SINGLE_CHOICE, R.array.answers_question_2, R.array.solution_question_2),
                getFromResource(R.string.title_question_3, QuestionType.SINGLE_CHOICE, R.array.answers_question_3, R.array.solution_question_3),
                getFromResource(R.string.title_question_4, QuestionType.FILL_ANSWER, R.array.answers_question_4, R.array.solution_question_4),
                getFromResource(R.string.title_question_5, QuestionType.MULTI_CHOICE, R.array.answers_question_5, R.array.solution_question_5),
                getFromResource(R.string.title_question_6, QuestionType.SINGLE_CHOICE, R.array.answers_question_6, R.array.solution_question_6),
                getFromResource(R.string.title_question_7, QuestionType.SINGLE_CHOICE, R.array.answers_question_7, R.array.solution_question_7),
                getFromResource(R.string.title_question_8, QuestionType.MULTI_CHOICE, R.array.answers_question_8, R.array.solution_question_8),
                getFromResource(R.string.title_question_9, QuestionType.SINGLE_CHOICE, R.array.answers_question_9, R.array.solution_question_9),
                getFromResource(R.string.title_question_10, QuestionType.FILL_ANSWER, R.array.answers_question_10, R.array.solution_question_10)
        );
    }

    private QuizQuestion getFromResource(@StringRes int titleId, QuestionType type, @ArrayRes int possibleAnswers, @ArrayRes int correctAnswers) {
        return new QuizQuestion(
                resources.getString(titleId),
                type,
                Arrays.asList(resources.getStringArray(possibleAnswers)),
                Arrays.asList(resources.getStringArray(correctAnswers))
        );
    }
}
