package com.example.cc13app;

import androidx.appcompat.app.AppCompatActivity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import java.util.ArrayList;
import java.util.Set;


public class Bluetooth extends AppCompatActivity{
    Button btnPaired;
    ListView devicelist;

    BluetoothAdapter myBluetooth = null;

    private Set<BluetoothDevice> pairedDevices;
    public static String EXTRA_ADDRESS = "device_address";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);

        btnPaired = (Button)findViewById(R.id.pairedDevicesButton);
        devicelist = (ListView)findViewById(R.id.pairedDeviceListView);

        myBluetooth = BluetoothAdapter.getDefaultAdapter();

        if(myBluetooth == null) {
            Toast.makeText(getApplicationContext(), "Bluetooth Adapter Not Available", Toast.LENGTH_LONG).show();
            finish();
        } else if(!myBluetooth.isEnabled()) {
            Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnBTon, 1);
        }
        btnPaired.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                pairedDevicesList();
            }
        });
    }

    private void pairedDevicesList() {
        pairedDevices = myBluetooth.getBondedDevices();
        ArrayList list = new ArrayList();

        if (pairedDevices.size() > 0)
            for(BluetoothDevice bt : pairedDevices)
                list.add(bt.getName() + "\n" + "Address: " + bt.getAddress());
        else
            Toast.makeText(getApplicationContext(), "No Paired Bluetooth Devices Found.", Toast.LENGTH_LONG).show();

        final ArrayAdapter adapter = new ArrayAdapter(this, R.layout.my_list_item , list);
        devicelist.setAdapter(adapter);
        devicelist.setOnItemClickListener(myListClickListener);
    }

    private AdapterView.OnItemClickListener myListClickListener = new AdapterView.OnItemClickListener() {
        public void onItemClick (AdapterView<?> av, View v, int arg2, long arg3) {
            String info = ((TextView) v).getText().toString();
            String address = info.substring(info.length() - 17);

            Intent serviceIntent = new Intent(Bluetooth.this, BluetoothServices.class);
            serviceIntent.putExtra(EXTRA_ADDRESS, address);
            startService(serviceIntent);
        }
    };

    @Override
    public void onBackPressed() {
        Intent i = new Intent(Bluetooth.this, MainActivity.class);
        startActivity(i);
        return;
    }
}