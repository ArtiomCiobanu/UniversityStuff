package com.android.lab3.data;

import androidx.annotation.LayoutRes;

import com.android.lab3.R;

public enum QuestionType {
    SINGLE_CHOICE(R.layout.dialog_single_choice),
    MULTI_CHOICE(R.layout.dialog_multi_choice),
    FILL_ANSWER(R.layout.dialog_fill_answer);

    private @LayoutRes
    final int layoutResId;

    QuestionType(int layoutResId) {
        this.layoutResId = layoutResId;
    }

    public int getLayoutResId() {
        return layoutResId;
    }
}
