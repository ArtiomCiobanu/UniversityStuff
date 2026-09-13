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
import com.android.lab3.databinding.DialogSingleChoiceBinding;
import com.android.lab3.ui.quiz.dialog.listener.DialogListener;
import com.google.android.material.radiobutton.MaterialRadioButton;

import java.util.Collections;

public class SingleChoiceQuizDialog extends DialogFragment {
    private DialogSingleChoiceBinding binding;
    private final QuizQuestion quizQuestion;
    private final DialogListener dialogListener;

    public SingleChoiceQuizDialog(QuizQuestion quizQuestion, DialogListener dialogListener) {
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
        binding = DialogSingleChoiceBinding.inflate(requireActivity().getLayoutInflater());
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

    private void registerUserInteraction() {
        binding.fabRequestQuiz.setOnClickListener(view -> {
            dialogListener.onNewQuestionRequested();
            requireDialog().dismiss();
        });
        binding.fabSubmitAnswer.setOnClickListener(view -> {
            MaterialRadioButton checkedRadioButton = binding.getRoot().findViewById(binding.rgSingleChoiceAnswers.getCheckedRadioButtonId());
            if (checkedRadioButton != null) {
                dialogListener.onAnswerSubmitted(quizQuestion, Collections.singletonList(checkedRadioButton.getText().toString()));
            }
        });
    }

    private void populateDialog() {
        binding.tvQuestionTitle.setText(quizQuestion.getTitle());
        for (String possibleAnswer : quizQuestion.getPossibleAnswers()) {
            MaterialRadioButton radioButton = new MaterialRadioButton(requireContext());
            radioButton.setTextColor(getResources().getColor(R.color.white, null));
            radioButton.setText(possibleAnswer);
            radioButton.setId(View.generateViewId());
            binding.rgSingleChoiceAnswers.addView(radioButton);
        }
    }
}
