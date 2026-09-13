package com.android.lab3.concurrent;

public interface TaskRunnerCallback<T> {
    void onSuccess(T result);
    void onError(Exception exception);
}
