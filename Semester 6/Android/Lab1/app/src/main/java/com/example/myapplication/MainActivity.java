package com.example.myapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void SetRedBackground(View view) {
        view.getRootView().setBackground(getResources().getDrawable(R.color.red));
    }

    public void SetYellowBackground(View view) {

        view.getRootView().setBackground(getResources().getDrawable(R.color.yellow));
    }

    public void SetGreenBackground(View view) {
        view.getRootView().setBackground(getResources().getDrawable(R.color.green));

    }
}