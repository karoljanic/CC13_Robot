package com.example.cc13app;

import androidx.appcompat.app.AppCompatActivity;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;


public class BatteryControl extends AppCompatActivity {
    LinearLayout layot;
    TextView stm32Text;
    ProgressBar stm32Bat;
    Button checkButton;

    boolean mBounded;
    BluetoothServices mServer = new BluetoothServices();

    int batteryNumber;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_battery_control);

        layot = (LinearLayout) findViewById(R.id.mainLayout);

        stm32Text = (TextView) findViewById(R.id.stm32BatText);

        stm32Bat = (ProgressBar) findViewById(R.id.stmBatBar);

        checkButton = (Button) findViewById(R.id.chechButton);

        stm32Bat.setMax(100);
        stm32Bat.setProgress(100);


        layot.setOnTouchListener(new OnSwipeTouchListener(BatteryControl.this) {
            public void onSwipeLeft() {
                Intent i = new Intent(BatteryControl.this, Control.class);
                startActivity(i);
            }
        });

        checkButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                sendNumber(10);
            }
        });
    }

    @Override
    protected void onStart() {
        super.onStart();

        Intent mIntent = new Intent(BatteryControl.this, BluetoothServices.class);
        bindService(mIntent, mConnection, BIND_AUTO_CREATE);
    };

    ServiceConnection mConnection = new ServiceConnection() {
        @Override
        public void onServiceDisconnected(ComponentName name) {
            Toast.makeText(BatteryControl.this, "Service is disconnected", 1000).show();
            mBounded = false;
            mServer = null;
        }

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            //Toast.makeText(Control.this, "Service is connected", 1000).show();
            mBounded = true;
            BluetoothServices.LocalBinder mLocalBinder = (BluetoothServices.LocalBinder)service;
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
            Integer bat = intent.getIntExtra("battery",-1/*default value*/);
            toast("Odebrano: "+((Integer)bat).toString());
            Log.e("Odebrano :", ((Integer)bat).toString());

        }
    };

    @Override
    protected void onPause() {
        // Unregister since the activity is not visible
        LocalBroadcastManager.getInstance(this)
                .unregisterReceiver(mMessageReceiver);
        super.onPause();
    }

    public void toast(String mess) {
        Toast.makeText(this, mess, Toast.LENGTH_SHORT).show();
    }

    public void send_newest(){
        batteryNumber = 1;
        sendNumber(10);
    }
}