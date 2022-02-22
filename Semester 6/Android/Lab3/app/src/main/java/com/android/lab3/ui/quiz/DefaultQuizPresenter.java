package com.android.lab3.ui.quiz;

import android.util.Log;

import com.android.lab3.concurrent.TaskRunner;
import com.android.lab3.concurrent.TaskRunnerCallback;
import com.android.lab3.data.QuestionType;
import com.android.lab3.data.QuizQuestion;
import com.android.lab3.storage.QuizStorage;
import com.android.lab3.view.QuizView;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;
import java.util.Random;

public class DefaultQuizPresenter implements QuizPresenter, TaskRunnerCallback<List<QuizQuestion>> {
    private final TaskRunner taskRunner;
    private final QuizStorage quizStorage;
    private final QuizView view;

    public DefaultQuizPresenter(TaskRunner taskRunner, QuizStorage quizStorage, QuizView view) {
        this.taskRunner = taskRunner;
        this.quizStorage = quizStorage;
        this.view = view;
    }

    @Override
    public void onSuccess(List<QuizQuestion> quizQuestions) {
        Random randomQuestion = new Random();
        view.onQuestionGenerated(quizQuestions.get(randomQuestion.nextInt(quizQuestions.size())));
    }

    @Override
    public void onError(Exception exception) {
        Log.e(TaskRunnerCallback.class.getName(), "onError: ", exception);
        view.onError();
    }

    @Override
    public void onQuizQuestionRequested() {
        taskRunner.execute(quizStorage::loadQuizQuestions, this);
    }

    @Override
    public void onAnsweredSubmitted(QuizQuestion quizQuestion, List<String> submittedAnswers) {
        List<String> correctAnswers = new ArrayList<>();
        List<String> answers = new ArrayList<>();
        for (String correctAnswer : quizQuestion.getCorrectAnswers()) {
            correctAnswers.add(correctAnswer.toLowerCase(Locale.ROOT));
        }
        for (String submittedAnswer : submittedAnswers) {
            answers.add(submittedAnswer.toLowerCase(Locale.ROOT));
        }
        if (quizQuestion.getType().equals(QuestionType.MULTI_CHOICE)) {
            if (correctAnswers.containsAll(answers)) {
                //todo compare correctly
                view.onAnswerCorrect();
            } else {
                view.onAnswerWrong();
            }
        } else if (answers.stream().anyMatch(correctAnswers::contains)) {
            view.onAnswerCorrect();
        } else {
            view.onAnswerWrong();
        }
    }
}
