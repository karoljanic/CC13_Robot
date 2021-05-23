package com.example.cc13app;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.telecom.Call;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.TextView;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;
import com.github.mikephil.charting.interfaces.datasets.ILineDataSet;
import com.github.mikephil.charting.listener.OnChartGestureListener;
import com.github.mikephil.charting.listener.OnChartValueSelectedListener;

import org.w3c.dom.Text;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.Map;


public class CallBackInformations extends AppCompatActivity {

    LinearLayout layot;
    Spinner informationSpinner, timePeriodSpinner;
    Button startButton, stopButton, displayButton, liveButton;
    LineChart chart;
    TextView infoText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_call_back_informations);

        layot = (LinearLayout) findViewById(R.id.mainLayout) ;
        informationSpinner = (Spinner) findViewById(R.id.informationSpinner);
        timePeriodSpinner = (Spinner) findViewById(R.id.timePeriodSpinner);
        startButton = (Button) findViewById(R.id.startButton);
        stopButton = (Button) findViewById(R.id.stopButton);
        displayButton = (Button) findViewById(R.id.displayButton);
        liveButton = (Button) findViewById(R.id.liveButton);
        chart = (LineChart) findViewById(R.id.chart);
        infoText = (TextView) findViewById(R.id.informationText);

        String[] arraySpinnerInformation = new String[]{
                "Acceleration in X axis",
                "Acceleration in Y axis",
                "Acceleration in Z axis",
                "Velocity in X axis",
                "Velocity in Y axis",
                "Velocity in Z axis",
                "Value of encoder 1",
                "Value of encoder 2"
        };

        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, arraySpinnerInformation);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        informationSpinner.setAdapter(adapter);

        String[] arraySpinnerTimePeriod = new String[]{
                "1 second",
                "2 seconds ",
                "3 seconds",
                "4 seconds",
                "5 seconds",
                "10 seconds",
                "15 seconds",
                "20 seconds",
                "30 seconds",
                "1 minute",
                "2 minutes",
                "3 minutes",
                "5 minutes",
                "10 minutes"
        };

        ArrayAdapter<String> adapter2 = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, arraySpinnerTimePeriod);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        timePeriodSpinner.setAdapter(adapter2);

        //chart.setOnChartGestureListener((OnChartGestureListener) CallBackInformations.this);
        //chart.setOnChartValueSelectedListener((OnChartValueSelectedListener) CallBackInformations.this);
        chart.setDragEnabled(true);
        chart.setFocusable(false);

        ArrayList<Entry> yValues = new ArrayList<>();
        /*yValues.add(new Entry(0,60f));
        yValues.add(new Entry(1,40));
        yValues.add(new Entry(2,34));
        yValues.add(new Entry(3,50));
        yValues.add(new Entry(4,12));
        yValues.add(new Entry(5,76f));
        yValues.add(new Entry(6,34f));
        yValues.add(new Entry(7,55f));*/
        LineDataSet set = new LineDataSet(yValues, "");

        set.setFillAlpha(110);
        set.setColor(Color.WHITE);
        set.setLineWidth(5f);
        set.setValueTextColor(Color.WHITE);
        set.setValueTextSize(0f);
        set.setCircleColor(Color.WHITE);
        set.setCircleHoleRadius(0f);


        ArrayList<ILineDataSet> dataSets = new ArrayList<>();
        dataSets.add(set);

        LineData data = new LineData(dataSets);

        chart.setData(data);

        layot.setOnTouchListener(new OnSwipeTouchListener(CallBackInformations.this) {
            public void onSwipeRight() {
                Intent i = new Intent(CallBackInformations.this, SetDistance.class);
                startActivity(i);
            }
        });

    }
}