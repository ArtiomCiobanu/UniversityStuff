package com.android.lab3.concurrent;

import android.os.Handler;
import android.os.Looper;

import java.util.concurrent.Callable;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;

public class AsyncTaskRunner<T> implements TaskRunner<T> {
    private final Executor executor = Executors.newSingleThreadExecutor();
    private final Handler handler = new Handler(Looper.getMainLooper());

    @Override
    public void execute(Callable<T> block, TaskRunnerCallback<T> callback) {
        executor.execute(() -> {
            try {
                T result = block.call();
                handler.post(() -> callback.onSuccess(result));
            } catch (Exception exception) {
                callback.onError(exception);
            }
        });
    }
}
