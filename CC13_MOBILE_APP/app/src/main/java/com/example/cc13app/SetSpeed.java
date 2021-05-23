package com.example.cc13app;

import android.content.Intent;
import android.graphics.fonts.FontVariationAxis;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

public class SetSpeed extends AppCompatActivity {

    float MAX = (float) 7.0;

    LinearLayout layot;
    Button startButton, stopButton, slowerButton, fasterButton, rightButton, leftButton;
    ProgressBar speedProgressBar;
    TextView speedText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_set_speed);

        layot = (LinearLayout) findViewById(R.id.mainLayout) ;
        startButton = (Button)findViewById(R.id.startButton);
        stopButton = (Button)findViewById(R.id.stopButton);
        slowerButton = (Button)findViewById(R.id.slowerButton);
        fasterButton = (Button)findViewById(R.id.fasterButton);
        rightButton = (Button)findViewById(R.id.rightButton);
        leftButton = (Button)findViewById(R.id.leftButton);

        speedProgressBar = (ProgressBar)findViewById(R.id.speedValueProgressBar);
        speedText = (TextView)findViewById(R.id.speedValue);

        MAX *= 10;
        speedProgressBar.setMax((int)MAX);

        final int[] speed = {0};
        setSpeed(speed[0]);

        fasterButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(speed[0] < MAX){
                    speed[0] += 1;
                    setSpeed(speed[0]);
                }
            }
        });

        slowerButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                if(speed[0] > 0){
                    speed[0] -= 1;
                    setSpeed(speed[0]);
                }
            }
        });

        layot.setOnTouchListener(new OnSwipeTouchListener(SetSpeed.this) {
            public void onSwipeRight() {
                Intent i = new Intent(SetSpeed.this, Control.class);
                startActivity(i);
            }
            public void onSwipeLeft() {
                Intent i = new Intent(SetSpeed.this, SetDistance.class);
                startActivity(i);
            }

        });
    }

    private void setSpeed(int speed){
        Integer V = (Integer)speed;
        float floatV = (float)speed;
        floatV /= 10.0;
        Float fv = (Float) floatV;
        speedText.setText(fv.toString());

        speedProgressBar.setProgress(speed);
    }
}