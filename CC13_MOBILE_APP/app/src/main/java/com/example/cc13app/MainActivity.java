package com.example.cc13app;

import androidx.appcompat.app.AppCompatActivity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;


public class MainActivity extends AppCompatActivity {
    private Button connect, exit;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        connect = (Button) findViewById(R.id.connectButton);
        exit = (Button) findViewById(R.id.exitButton);

        connect.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Intent intent = new Intent(getApplicationContext(), Bluetooth.class);
                startActivity(intent);
            }
        });;

        exit.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                finish();
                System.exit(0);
            }
        });;
    }

    @Override
    public void onBackPressed() {
        return;
    }
}