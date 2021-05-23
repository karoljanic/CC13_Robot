package com.example.cc13app;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;
import android.widget.Button;
import android.widget.CheckBox;

import com.example.cc13app.BluetoothServices.LocalBinder;


public class Control extends AppCompatActivity{
    RelativeLayout layout_joystick1, layout_joystick2;
    LinearLayout layot;
    JoyStickClass joystick1;
    JoyStickClass joystick2;
    Switch onOff1, onOff2;
    CheckBox r1,g1,b1, r2,g2,b2;

    boolean mBounded;
    BluetoothServices mServer = new BluetoothServices();

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_control);
        

        layout_joystick1 = (RelativeLayout) findViewById(R.id.bigJoy);
        layout_joystick2 = (RelativeLayout) findViewById(R.id.smallJoy);
        layot = (LinearLayout) findViewById(R.id.mainLayout) ;
        onOff1 = (Switch) findViewById(R.id.onOff1);
        onOff2 = (Switch) findViewById(R.id.onOff2);
        r1 = (CheckBox) findViewById(R.id.checkBoxR1);
        g1 = (CheckBox) findViewById(R.id.checkBoxG1);
        b1 = (CheckBox) findViewById(R.id.checkBoxB1);
        r2 = (CheckBox) findViewById(R.id.checkBoxR2);
        g2 = (CheckBox) findViewById(R.id.checkBoxG2);
        b2 = (CheckBox) findViewById(R.id.checkBoxB2);

        DisplayMetrics displayMetrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(displayMetrics);

        joystick1 = new JoyStickClass(getApplicationContext(), layout_joystick1, R.drawable.image_button);
        joystick1.setStickSize(100, 100);
        joystick1.setLayoutSize(500, 500);
        joystick1.setLayoutAlpha(150);
        joystick1.setStickAlpha(100);
        joystick1.setOffset(50);
        joystick1.setMinimumDistance(0);

        joystick2 = new JoyStickClass(getApplicationContext(), layout_joystick2, R.drawable.image_button);
        joystick2.setStickSize(60, 60);
        joystick2.setLayoutSize(300, 300);
        joystick2.setLayoutAlpha(150);
        joystick2.setStickAlpha(100);
        joystick2.setOffset(30);
        joystick2.setMinimumDistance(0);

        r1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                sendNumber(11);
                if(onOff1.isChecked()) {
                    sendLedInfo(r1.isChecked(), g1.isChecked(), b1.isChecked());
                }
                else {
                    sendLedInfo(false, false, false);
                }
            }
        });
        g1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                sendNumber(11);
                if(onOff1.isChecked()) {
                    sendLedInfo(r1.isChecked(), g1.isChecked(), b1.isChecked());
                }
                else {
                    sendLedInfo(false, false, false);
                }
            }
        });
        b1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                sendNumber(11);
                if(onOff1.isChecked()) {
                    sendLedInfo(r1.isChecked(), g1.isChecked(), b1.isChecked());
                }
                else {
                    sendLedInfo(false, false, false);
                }
            }
        });

        onOff1.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
            sendNumber(11);
            if(onOff1.isChecked()) {
                 sendLedInfo(r1.isChecked(), g1.isChecked(), b1.isChecked());
            }
            else {
                 sendLedInfo(false, false, false);
            }
            }
        });

        r2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                sendNumber(12);
                if(onOff2.isChecked()) {
                    sendLedInfo(r2.isChecked(), g2.isChecked(), b2.isChecked());
                }
                else {
                    sendLedInfo(false, false, false);
                }
            }
        });
        g2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                sendNumber(12);
                if(onOff2.isChecked()) {
                    sendLedInfo(r2.isChecked(), g2.isChecked(), b2.isChecked());
                }
                else {
                    sendLedInfo(false, false, false);
                }
            }
        });
        b2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                sendNumber(12);
                if(onOff2.isChecked()) {
                    sendLedInfo(r2.isChecked(), g2.isChecked(), b2.isChecked());
                }
                else {
                    sendLedInfo(false, false, false);
                }
            }
        });

        onOff2.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
             sendNumber(12);
            if(onOff2.isChecked()) {
                 sendLedInfo(r2.isChecked(), g2.isChecked(), b2.isChecked());
            }
            else {
                 sendLedInfo(false, false, false);
            }
            }
        });

        layout_joystick1.setOnTouchListener(new OnTouchListener() {
            public boolean onTouch(View arg0, MotionEvent arg1) {
            joystick1.drawStick(arg1);
            sendNumber(21);
            if(arg1.getAction() == MotionEvent.ACTION_UP)
                sendJoyInfo(0, 0);
            else
                sendJoyInfo(joystick1.getX(), joystick1.getY());
            return true;
            }
        });

        layout_joystick2.setOnTouchListener(new OnTouchListener() {
            public boolean onTouch(View arg0, MotionEvent arg1) {
                joystick2.drawStick(arg1);
                sendNumber(22);
                if(arg1.getAction() == MotionEvent.ACTION_UP){

                }
                else
                    sendJoyInfo(joystick2.getX(), joystick2.getY());
                return true;
            }
        });

        layot.setOnTouchListener(new OnSwipeTouchListener(Control.this) {
            public void onSwipeLeft() {
                Intent i = new Intent(Control.this, SetSpeed.class);
                startActivity(i);
            }

            public void onSwipeRight() {
                Intent i = new Intent(Control.this, BatteryControl.class);
                startActivity(i);
            }

        });
    }

    @Override
    protected void onStart() {
        super.onStart();

        Intent mIntent = new Intent(Control.this, BluetoothServices.class);
        bindService(mIntent, mConnection, BIND_AUTO_CREATE);
    };

    ServiceConnection mConnection = new ServiceConnection() {
        @Override
        public void onServiceDisconnected(ComponentName name) {
            Toast.makeText(Control.this, "Service is disconnected", 1000).show();
            mBounded = false;
            mServer = null;
        }

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            //Toast.makeText(Control.this, "Service is connected", 1000).show();
            mBounded = true;
            LocalBinder mLocalBinder = (LocalBinder)service;
            mServer = mLocalBinder.getService();
        }
    };

    @Override
    protected void onStop() {
        super.onStop();
        if(mBounded) {
            unbindService(mConnection);
            mBounded = false;
        }
    };

    private void sendNumber(int number){
        byte result = (byte) number;
        Integer res = (int)result;
        mServer.sendByte(result);
        //toast(res.toString());
    }

    private void sendLedInfo(boolean r, boolean g, boolean b){
        int result = 0;
        if(r){
            result |= 1;
        }
        if(g){
            result |= 2;
        }
        if(b){
            result |= 4;
        }
        sendNumber(result);
    }

    private void sendJoyInfo(int x, int y){
        sendNumber(x);
        sendNumber(y);
    }

    @Override
    public void onBackPressed() {
        disconnect();
        Intent i = new Intent(Control.this, Bluetooth.class);
        startActivity(i);
        return;
    }

    public void disconnect() {
        mServer.stopService(getIntent());
    }

    public void toast(String mess) {
        Toast.makeText(this, mess, Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onResume() {
        super.onResume();
        // This registers mMessageReceiver to receive messages.
        LocalBroadcastManager.getInstance(this)
                .registerReceiver(mMessageReceiver,
                        new IntentFilter("my-integer"));
    }

    private BroadcastReceiver mMessageReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            // Extract data included in the Intent
            Integer yourInteger = intent.getIntExtra("battery",-1);
            toast(yourInteger.toString());
        }
    };

    @Override
    protected void onPause() {
        // Unregister since the activity is not visible
        LocalBroadcastManager.getInstance(this)
                .unregisterReceiver(mMessageReceiver);
        super.onPause();
    }

}