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
import com.android.lab3.databinding.DialogMultiChoiceBinding;
import com.android.lab3.ui.quiz.dialog.listener.DialogListener;
import com.google.android.material.checkbox.MaterialCheckBox;

import java.util.ArrayList;
import java.util.List;

public class MultiChoiceQuizDialog extends DialogFragment {
    private DialogMultiChoiceBinding binding;
    private final QuizQuestion quizQuestion;
    private final DialogListener dialogListener;
    private List<String> submittedAnswers = new ArrayList<>();

    public MultiChoiceQuizDialog(QuizQuestion quizQuestion, DialogListener dialogListener) {
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
        binding = DialogMultiChoiceBinding.inflate(requireActivity().getLayoutInflater());
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
        for (String possibleAnswer : quizQuestion.getPossibleAnswers()) {
            MaterialCheckBox checkBox = new MaterialCheckBox(requireContext());
            checkBox.setTextColor(getResources().getColor(R.color.white, null));
            checkBox.setText(possibleAnswer);
            checkBox.setId(View.generateViewId());
            binding.layoutMultiChoiceAnswers.addView(checkBox);
        }
    }

    private void setOnCheckBoxListener() {
        List<String> submittedAnswers = new ArrayList<>();
        for (int index = 0; index < binding.layoutMultiChoiceAnswers.getChildCount(); index++) {
            MaterialCheckBox checkBox = (MaterialCheckBox) binding.layoutMultiChoiceAnswers.getChildAt(index);
            checkBox.setOnCheckedChangeListener((compoundButton, isChecked) -> {
                if (isChecked) {
                    submittedAnswers.add(checkBox.getText().toString());
                } else {
                    submittedAnswers.remove(checkBox.getText().toString());
                }
                this.submittedAnswers = submittedAnswers;
            });
        }
    }

    private void registerUserInteraction() {
        setOnCheckBoxListener();
        binding.fabRequestQuiz.setOnClickListener(view -> {
            dialogListener.onNewQuestionRequested();
            requireDialog().dismiss();
        });
        binding.fabSubmitAnswer.setOnClickListener(view -> {
            dialogListener.onAnswerSubmitted(quizQuestion, submittedAnswers);
        });
    }
}
