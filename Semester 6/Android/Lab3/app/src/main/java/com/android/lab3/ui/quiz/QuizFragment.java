package com.android.lab3.ui.quiz;

import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.android.lab3.DependencyContainer;
import com.android.lab3.R;
import com.android.lab3.data.QuizQuestion;
import com.android.lab3.databinding.FragmentQuizBinding;
import com.android.lab3.ui.quiz.dialog.FillAnswerQuizDialog;
import com.android.lab3.ui.quiz.dialog.MultiChoiceQuizDialog;
import com.android.lab3.ui.quiz.dialog.SingleChoiceQuizDialog;
import com.android.lab3.ui.quiz.dialog.listener.DialogListener;
import com.android.lab3.view.QuizView;
import com.google.android.material.snackbar.Snackbar;

import java.util.List;

public class QuizFragment extends Fragment implements QuizView, DialogListener {
    private FragmentQuizBinding binding;
    private QuizPresenter presenter;

    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        binding = FragmentQuizBinding.inflate(inflater, container, false);
        return binding.getRoot();
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        presenter = new DefaultQuizPresenter(DependencyContainer.getINSTANCE().taskRunner(), DependencyContainer.getINSTANCE().quizStorage(), this);
        presenter.onQuizQuestionRequested();
    }

    @Override
    public void onAnswerSubmitted(QuizQuestion quizQuestion, List<String> submittedAnswers) {
        presenter.onAnsweredSubmitted(quizQuestion, submittedAnswers);
    }

    @Override
    public void onNewQuestionRequested() {
        presenter.onQuizQuestionRequested();
    }

    @Override
    public void onQuestionGenerated(QuizQuestion quizQuestion) {
        switch (quizQuestion.getType()) {
            case SINGLE_CHOICE:
                new SingleChoiceQuizDialog(quizQuestion, this).show(getParentFragmentManager(), SingleChoiceQuizDialog.class.getName());
                break;
            case MULTI_CHOICE:
                new MultiChoiceQuizDialog(quizQuestion, this).show(getParentFragmentManager(), MultiChoiceQuizDialog.class.getName());
                break;
            case FILL_ANSWER:
                new FillAnswerQuizDialog(quizQuestion, this).show(getParentFragmentManager(), FillAnswerQuizDialog.class.getName());
                break;
        }
    }

    @Override
    public void onAnswerCorrect() {
        Snackbar.make(binding.getRoot(), R.string.message_answer_correct, Snackbar.LENGTH_SHORT).show();
    }

    @Override
    public void onAnswerWrong() {
        Snackbar.make(binding.getRoot(), R.string.error_wrong_answer, Snackbar.LENGTH_SHORT).show();
    }

    @Override
    public void onError() {
        Snackbar.make(binding.getRoot(), R.string.error_unknown_error_occurred, Snackbar.LENGTH_SHORT).show();
    }

    public static QuizFragment newInstance() {
        return new QuizFragment();
    }
}