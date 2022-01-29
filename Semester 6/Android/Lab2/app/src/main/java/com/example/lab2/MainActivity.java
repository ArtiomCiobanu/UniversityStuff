package com.example.lab2;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.ScrollView;

import java.util.ArrayList;
import java.util.Dictionary;

public class MainActivity extends AppCompatActivity {

    private final FlagQuestion[] flagQuestions = {
            new FlagQuestion("\uD83C\uDDF2\uD83C\uDDE9", "Moldova"),
            new FlagQuestion("\uD83C\uDDE9\uD83C\uDDEA", "Germany"),
            new FlagQuestion("\uD83C\uDDF7\uD83C\uDDFA", "Russia")
    };

    private ScrollView questionScrollView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        InitializeFlagQuestions();
        CreateQuestionList();

        questionScrollView = findViewById(R.id.QuestionScrollView);
    }

    private void InitializeFlagQuestions() {
        flagQuestions[0].Answers = new String[]{"Moldova", "Germany", "France", "USA"};
        flagQuestions[1].Answers = new String[]{"Russia", "Germany", "Moldova", "Ukraine"};
        flagQuestions[2].Answers = new String[]{"Australia", "Russia", "France", "Spain"};
    }

    private void CreateQuestionList() {
        for (int i = 0; i < flagQuestions.length; i++) {
            Button button = GetButtonFromQuestion(flagQuestions[i], i);
            questionScrollView.addView(button);
        }

    }

    private Button GetButtonFromQuestion(FlagQuestion flagQuestion, int questionNumber) {
        Button button = new Button(this);
        button.setText("Question " + questionNumber);

        button.setOnClickListener(view -> {
            Intent intent = new Intent(MainActivity.this, StatisticActivity.class);
            startActivityForResult(intent, 1);
        });

        return button;
    }
}
