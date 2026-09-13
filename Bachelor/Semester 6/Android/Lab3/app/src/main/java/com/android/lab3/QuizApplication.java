package com.android.lab3;

import android.app.Application;

public class QuizApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        DependencyContainer.init(this);
    }
}
