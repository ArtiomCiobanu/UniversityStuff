package com.android.lab3.concurrent;

import java.util.concurrent.Callable;

public interface TaskRunner<T> {
    void execute(Callable<T> block, TaskRunnerCallback<T> callback);
}

