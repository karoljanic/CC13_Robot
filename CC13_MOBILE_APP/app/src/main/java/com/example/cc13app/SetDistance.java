
package com.example.cc13app;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.TextClock;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Set;

public class SetDistance extends AppCompatActivity {

    LinearLayout layot;
    ListView commands;
    Button forwardButton, backwardButton, leftButton, rightButton, startButton, abortButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_set_distance);

        layot = (LinearLayout) findViewById(R.id.mainLayout);
        commands = (ListView) findViewById(R.id.commands);
        forwardButton = (Button) findViewById(R.id.forwardButton);
        backwardButton = (Button) findViewById(R.id.backwardButton);
        rightButton = (Button) findViewById(R.id.rightButton);
        leftButton = (Button) findViewById(R.id.leftButton);
        startButton = (Button) findViewById(R.id.startButton);
        abortButton = (Button) findViewById(R.id.abortButton);

        String[] array = {};
        final ArrayList<String> commandsAL = new ArrayList<String>(Arrays.asList(array));
        final ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, R.layout.my_list_item, commandsAL);

        commands.setAdapter(adapter);
        commands.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, final int position, long id) {
                final String selectedItem = (String) parent.getItemAtPosition(position);

                final EditText edtText = new EditText(SetDistance.this);
                AlertDialog.Builder builder = new AlertDialog.Builder(SetDistance.this, R.style.AlertDialog);
                builder.setTitle(selectedItem);
                builder.setMessage("Set value of move!");
                builder.setCancelable(false);
                builder.setView(edtText);
                builder.setNeutralButton("Set", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        String last;
                        last = selectedItem.substring(selectedItem.length()-1, selectedItem.length());
                        commandsAL.set(position, selectedItem.substring(0, selectedItem.indexOf(':')+1)+"  "+edtText.getText()+last);
                        commands.setAdapter(adapter);
                    }
                });
                builder.show();

            }
        });

        forwardButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                commandsAL.add("Forward: ...m");
                commands.setAdapter(adapter);
            }
        });

        backwardButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                commandsAL.add("Backward: ...m");
                commands.setAdapter(adapter);
            }
        });

        rightButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                commandsAL.add("Right: ..." + (char) 0x00B0);
                commands.setAdapter(adapter);
            }
        });

        leftButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                commandsAL.add("Left: ..." + (char) 0x00B0);
                commands.setAdapter(adapter);
            }
        });

        abortButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                commandsAL.clear();
                commands.setAdapter(adapter);
            }
        });


        layot.setOnTouchListener(new OnSwipeTouchListener(SetDistance.this) {
            public void onSwipeRight() {
                Intent i = new Intent(SetDistance.this, SetSpeed.class);
                startActivity(i);
            }

            public void onSwipeLeft() {
                Intent i = new Intent(SetDistance.this, CallBackInformations.class);
                startActivity(i);
            }

        });

    }

    private void toast(String s) {
        Toast.makeText(getApplicationContext(),s,Toast.LENGTH_LONG).show();
    }
}