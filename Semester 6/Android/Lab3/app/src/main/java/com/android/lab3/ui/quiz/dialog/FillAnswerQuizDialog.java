package com.android.lab3.ui.quiz.dialog;

import android.app.Dialog;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AlertDialog;
import androidx.fragment.app.DialogFragment;

import com.android.lab3.R;
import com.android.lab3.data.QuizQuestion;
import com.android.lab3.databinding.DialogFillAnswerBinding;
import com.android.lab3.ui.quiz.dialog.listener.DialogListener;

import java.util.Collections;

public class FillAnswerQuizDialog extends DialogFragment {
    private DialogFillAnswerBinding binding;
    private final QuizQuestion quizQuestion;
    private final DialogListener dialogListener;

    public FillAnswerQuizDialog(QuizQuestion quizQuestion, DialogListener dialogListener) {
        this.quizQuestion = quizQuestion;
        this.dialogListener = dialogListener;
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return binding.getRoot();
    }

    @NonNull
    @Override
    public Dialog onCreateDialog(@Nullable Bundle savedInstanceState) {
        binding = DialogFillAnswerBinding.inflate(requireActivity().getLayoutInflater());
        AlertDialog alertDialog = new AlertDialog.Builder(requireContext(), R.style.Theme_Musiquiz_NoBackgroundDialog)
                .setView(binding.getRoot())
                .setCancelable(false).create();
        alertDialog.getWindow().setDimAmount(0);
        alertDialog.setCanceledOnTouchOutside(false);
        return alertDialog;
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        populateDialog();
        registerUserInteraction();
    }

    private void populateDialog() {
        binding.tvQuestionTitle.setText(quizQuestion.getTitle());
    }

    private void registerUserInteraction() {
        binding.fabRequestQuiz.setOnClickListener(view -> {
            dialogListener.onNewQuestionRequested();
            requireDialog().dismiss();
        });
        binding.fabSubmitAnswer.setOnClickListener(view -> {
            dialogListener.onAnswerSubmitted(quizQuestion, Collections.singletonList(binding.etQuestionAnswer.getText().toString()));
        });
    }
}
