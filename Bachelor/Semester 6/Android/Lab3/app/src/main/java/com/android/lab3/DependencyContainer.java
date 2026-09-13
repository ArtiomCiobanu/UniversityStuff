package com.android.lab3;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.Resources;

import com.android.lab3.concurrent.AsyncTaskRunner;
import com.android.lab3.concurrent.TaskRunner;
import com.android.lab3.storage.DefaultQuizStorage;
import com.android.lab3.storage.QuizStorage;

@SuppressLint("StaticFieldLeak")
public class DependencyContainer {
    private static DependencyContainer INSTANCE = null;
    private static Context _context;

    private DependencyContainer() {
    }

    public static DependencyContainer getINSTANCE() {
        if (INSTANCE == null) {
            INSTANCE = new DependencyContainer();
        }

        return INSTANCE;
    }

    public static void init(Context context) {
        _context = context;
    }

    public Resources resources() {
        return _context.getResources();
    }

    public QuizStorage quizStorage() {
        return new DefaultQuizStorage(resources());
    }

    public <T> TaskRunner<T> taskRunner() {
        return new AsyncTaskRunner<>();
    }
}
