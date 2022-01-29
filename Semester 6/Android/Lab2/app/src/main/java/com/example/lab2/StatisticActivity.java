package com.example.lab2;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;

public class StatisticActivity extends AppCompatActivity {

    private String flag;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_statistic);

        TextView textView = this.findViewById(R.id.textView);
        LinearLayout linearLayout = this.findViewById(R.id.linearLayout);

        Bundle b = this.getIntent().getExtras();

        int questionId = b.getInt("questionId");
        this.setTitle("Question " + questionId);

        flag = b.getString("flag");
        textView.setText(flag);

        String[] answers = b.getStringArray("answers");
        for (String answer : answers) {
            Button button = GetAnswerButton(answer);
            linearLayout.addView(button);
        }
    }

    private Button GetAnswerButton(String answer) {
        Button button = new Button(this);
        button.setText(answer);
        button.setWidth(300);
        button.setHeight(100);

        button.setOnClickListener(view -> {
            Intent intent = new Intent();
            intent.putExtra("answer", answer);
            intent.putExtra("flag", flag);

            this.setResult(RESULT_OK, intent);
            finish();
        });

        return button;
    }
}
