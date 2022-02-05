package com.example.lab2;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

/*Создать игру «Флаги» – есть набор стран,
есть набор флагов, на экране появляется флаг и 4 кнопки со странами,
по выбору правильной кнопки – переход к следующему из N экранов.
Дополнительные возможности: статистика выигрышей/проигрышей, экран настройки количества экранов.*/
public class MainActivity extends AppCompatActivity {

    private final FlagQuestion[] flagQuestions = {
            new FlagQuestion("\uD83C\uDDF2\uD83C\uDDE9", "Moldova"),
            new FlagQuestion("\uD83C\uDDE9\uD83C\uDDEA", "Germany"),
            new FlagQuestion("\uD83C\uDDF7\uD83C\uDDFA", "Russia")
    };

    private LinearLayout linearLayout;
    private TextView resultTextView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        linearLayout = this.findViewById(R.id.linearLayout);
        resultTextView = this.findViewById(R.id.resultTextView);

        InitializeFlagQuestions();
        CreateQuestionList();

        UpdateResults();
    }

    private void InitializeFlagQuestions() {
        flagQuestions[0].Answers = new String[]{"Moldova", "Germany", "France", "USA"};
        flagQuestions[1].Answers = new String[]{"Russia", "Germany", "Moldova", "Ukraine"};
        flagQuestions[2].Answers = new String[]{"Australia", "Russia", "France", "Spain"};
    }

    private void CreateQuestionList() {
        for (int i = 0; i < flagQuestions.length; i++) {
            Button button = GetButtonFromQuestion(flagQuestions[i], i);
            linearLayout.addView(button);
        }
    }

    private Button GetButtonFromQuestion(FlagQuestion flagQuestion, int questionId) {
        Button button = new Button(this);
        button.setText("Question " + questionId);
        button.setWidth(300);
        button.setHeight(100);

        button.setOnClickListener(view -> {
            /*Bundle bundle = new Bundle();
            bundle.putString("flag", flagQuestion.Flag);
            bundle.putStringArray("answers", flagQuestion.Answers);
            bundle.putInt("questionId", questionId);*/

            Intent intent = new Intent(MainActivity.this, QuestionActivity.class);
            //intent.putExtras(bundle);
            intent.putExtra("flag", flagQuestion.Flag);
            intent.putExtra("answers", flagQuestion.Answers);
            intent.putExtra("questionId", questionId);
            startActivityForResult(intent, 1);
        });

        return button;
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        String flag = data.getStringExtra("flag");
        String answer = data.getStringExtra("answer");

        for (FlagQuestion flagQuestion : flagQuestions) {
            if (flagQuestion.Flag.equals(flag)
             && flagQuestion.CorrectAnswer.equals(answer)) {
                flagQuestion.AnsweredCorrectly = true;
            }
        }

        UpdateResults();
    }

    private void UpdateResults() {
        int total = 0;

        for (FlagQuestion flagQuestion : flagQuestions) {
            if (flagQuestion.AnsweredCorrectly) {
                total++;
            }
        }

        resultTextView.setText("Total correct answers: " + total);
    }
}
