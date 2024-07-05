package com.ongolly.smokerv2interface;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private Button smoker_control;
    private Button sensor_readings;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        smoker_control = (Button) findViewById(R.id.goto_smoker_control);
        sensor_readings = (Button) findViewById(R.id.goto_detection);

        smoker_control.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Intent intent = new Intent(MainActivity.this, RobotControlActivity.class);
                startActivity(intent);
            }
        });

        sensor_readings.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v){
                Intent intent = new Intent(MainActivity.this, SensorViewActivity.class);
                startActivity(intent);
            }
        });
    }
}