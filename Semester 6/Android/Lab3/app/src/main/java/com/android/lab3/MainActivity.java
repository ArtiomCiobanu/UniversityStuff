package com.android.lab3;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.android.lab3.ui.splash.SplashFragment;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getSupportFragmentManager()
                .beginTransaction()
                .replace(R.id.fragment_container, SplashFragment.newInstance())
                .commit();
    }
}