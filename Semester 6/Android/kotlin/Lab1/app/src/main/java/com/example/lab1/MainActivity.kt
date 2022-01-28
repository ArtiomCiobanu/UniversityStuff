package com.example.lab1

import android.graphics.Color.red
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.view.View

class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
    }

    fun SetRedBackground(view: View) {
        view.rootView.setBackgroundColor(resources.getColor(R.color.red));
    }

    fun SetYellowBackground(view: View) {
        view.rootView.setBackgroundColor(resources.getColor(R.color.yellow));
    }

    fun SetGreenBackground(view: View) {
        view.rootView.setBackgroundColor(resources.getColor(R.color.green));
    }

}
