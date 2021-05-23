package com.example.cc13app;

import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.ComponentName;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;
import androidx.annotation.Nullable;
import androidx.localbroadcastmanager.content.LocalBroadcastManager;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;
import java.util.Vector;


public class BluetoothServices extends Service {

    public BluetoothAdapter mBluetoothAdapter;
    public static String EXTRA_ADDRESS = "device_address";
    public static final String B_DEVICE = "MY DEVICE";
    public static final String B_UUID = "00001101-0000-1000-8000-00805f9b34fb";
    // 00000000-0000-1000-8000-00805f9b34fb
    public static final int STATE_NONE = 0;
    public static final int STATE_LISTEN = 1;
    public static final int STATE_CONNECTING = 2;
    public static final int STATE_CONNECTED = 3;

    private ConnectBtThread mConnectThread;
    private static ConnectedBtThread mConnectedThread;

    private static Handler mHandler = null;
    public static int mState = STATE_NONE;
    public static String deviceName;
    public static BluetoothDevice sDevice = null;
    public Vector<Byte> packData = new Vector<>(2048);
    //IBinder mIBinder = new LocalBinder();


    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        //mHandler = getApplication().getHandler();
        return mBinder;
    }

    public void toast(String mess) {
        Toast.makeText(this, mess, Toast.LENGTH_SHORT).show();
    }

    private final IBinder mBinder = new LocalBinder();

    public void onServiceDisconnected(ComponentName name) {

    }

    public class LocalBinder extends Binder {
        BluetoothServices getService() {
            // Return this instance of LocalSernvice so clients can call public methods
            return BluetoothServices.this;
        }
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        String deviceg = intent.getStringExtra(EXTRA_ADDRESS);
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        connectToDevice(deviceg);

        return START_STICKY;
    }

    private synchronized void connectToDevice(String macAddress) {
        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(macAddress);
        if (mState == STATE_CONNECTING) {
            if (mConnectThread != null) {
                mConnectThread.cancel();
                mConnectThread = null;
            }
        }
        if (mConnectedThread != null) {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }
        mConnectThread = new ConnectBtThread(device);
        toast("connecting to "+macAddress);
        mConnectThread.start();
        setState(STATE_CONNECTING);

    }

    private void setState(int state) {
        mState = state;
        if (mHandler != null) {
            // mHandler.obtainMessage();
        }
    }

    public synchronized void stop() {
        setState(STATE_NONE);
        if (mConnectThread != null) {
            mConnectThread.cancel();
            mConnectThread = null;
        }
        if (mConnectedThread != null) {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }
        if (mBluetoothAdapter != null) {
            mBluetoothAdapter.cancelDiscovery();
        }

        stopSelf();
    }

    public void sendData(String message) {
        if (mConnectedThread != null) {
            mConnectedThread.write(message.getBytes());
            //toast("sent data");
        } else {
            Toast.makeText(BluetoothServices.this, "Failed to send data", Toast.LENGTH_SHORT).show();
        }
    }

    public void sendByte(byte number){
        if (mConnectedThread != null) {
            mConnectedThread.writeByte(number);
            //toast("sent data");
        } else {
            Toast.makeText(BluetoothServices.this, "Failed to send data", Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public boolean stopService(Intent name) {
        setState(STATE_NONE);

        if (mConnectThread != null) {
            mConnectThread.cancel();
            mConnectThread = null;
        }

        if (mConnectedThread != null) {
            mConnectedThread.cancel();
            mConnectedThread = null;
        }

        mBluetoothAdapter.cancelDiscovery();
        return super.stopService(name);
    }

private synchronized void connected(BluetoothSocket mmSocket){

    if (mConnectThread != null){
        mConnectThread.cancel();
        mConnectThread = null;
    }
    if (mConnectedThread != null){
        mConnectedThread.cancel();
        mConnectedThread = null;
    }

    mConnectedThread = new ConnectedBtThread(mmSocket);
    mConnectedThread.start();


    setState(STATE_CONNECTED);
}

    private class ConnectBtThread extends Thread {
        private final BluetoothSocket mSocket;
        private final BluetoothDevice mDevice;

        public ConnectBtThread(BluetoothDevice device) {
            Log.d("ConnectBT_Thread","zaczynam");
            mDevice = device;
            BluetoothSocket socket = null;
            try {
                socket = device.createInsecureRfcommSocketToServiceRecord(UUID.fromString(B_UUID));
            } catch (IOException e) {
                e.printStackTrace();
            }
            mSocket = socket;
        }

        @Override
        public void run() {
            mBluetoothAdapter.cancelDiscovery();
            try {
                mSocket.connect();
                Log.d("service", "connect thread run method (connected)");
                SharedPreferences pre = getSharedPreferences("BT_NAME", 0);
                pre.edit().putString("bluetooth_connected", mDevice.getName()).apply();

                Intent dialogIntent = new Intent(BluetoothServices.this, Control.class);
                dialogIntent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                startActivity(dialogIntent);

            } catch (IOException e) {

                try {
                    mSocket.close();
                    Log.d("service", "connect thread run method ( close function)");
                } catch (IOException e1) {
                    e1.printStackTrace();
                }
                e.printStackTrace();
            }
            //connected(mSocket);
            mConnectedThread = new ConnectedBtThread(mSocket);
            mConnectedThread.start();
        }

        public void cancel() {

            try {
                mSocket.close();
                Log.d("service", "connect thread cancel method");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private class ConnectedBtThread extends Thread {
        private final BluetoothSocket cSocket;
        private final InputStream inS;
        private final OutputStream outS;

        private byte[] buffer;

        public ConnectedBtThread(BluetoothSocket socket) {
            cSocket = socket;
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try {
                tmpIn = socket.getInputStream();

            } catch (IOException e) {
                e.printStackTrace();
            }

            try {
                tmpOut = socket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }

            inS = tmpIn;
            outS = tmpOut;
        }

        @Override
        public void run() {
            buffer = new byte[1024];
            int mByte;
            while(true) {
                try {
                    mByte = inS.read(buffer);
                    sendMessage(mByte);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                Log.d("service", "connected thread run method 777");
            }
        }

        public void write(byte[] buff) {
            try {
                outS.write(buff);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void writeByte(byte number){     // byte from -128 to 127
            try {
                byte result[] = {number};
                outS.write(result);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        private void cancel() {
            try {
                cSocket.close();
                Log.d("service", "connected thread cancel method");
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    @Override
    public void onDestroy() {
        this.stop();
        super.onDestroy();
    }

    // Supposing that your value is an integer declared somewhere as: int myInteger;
    private void sendMessage(int myInteger) {
        // The string "my-integer" will be used to filer the intent
        Intent intent = new Intent("my-integer");
        // Adding some data
        intent.putExtra("battery", myInteger);
        LocalBroadcastManager.getInstance(this).sendBroadcast(intent);
    }
}