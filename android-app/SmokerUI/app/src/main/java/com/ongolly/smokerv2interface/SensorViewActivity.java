package com.ongolly.smokerv2interface;

import android.graphics.Canvas;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;

import androidx.appcompat.app.AppCompatActivity;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.JsonRequest;
import com.android.volley.toolbox.Volley;
import android.graphics.Canvas;
import android.graphics.Paint;

import org.json.JSONException;
import org.json.JSONObject;

import java.lang.reflect.Array;
import java.util.ArrayList;

public class SensorViewActivity extends AppCompatActivity {
    private RequestQueue queue;
    final long delayTime = 3000;
    private Handler handler;
    private SensorOutput display;
    final private String server_url = "http://192.168.62.31:5000/smokerV2";//"http://10.0.0.7:5000/smokerV2";
    @Override
    protected void onCreate(Bundle savedInstanceBundle){
        super.onCreate(savedInstanceBundle);
        setContentView(R.layout.sensor_detection);

        queue = Volley.newRequestQueue(SensorViewActivity.this);
        display = (SensorOutput) findViewById(R.id.map_display);

        ArrayList<DistanceReading> dummy = new ArrayList<DistanceReading>();

        handler = new Handler();

        //Set a threshold distance where I would say that the sensors won't scan any
        //  further. In this case, the farthest distance that I would allow for a
        //  reading to be visible is at around 150cm. Any further and the reading won't
        //  be reflected.
        for (int i = 45; i < 136; i+=15){
            double thresh = 100;
            if (Math.abs(i-90) == 0){
                thresh = 150;
            } else if (Math.abs(i-90) == 15){
                thresh = 150 / Math.cos(Math.toRadians(15));
            } else if (Math.abs(i-90) == 30){
                thresh = 140;
            }
            dummy.add(new DistanceReading(Math.toRadians(i), (float) thresh));
        }
        display.addReadings(dummy);
    }

    @Override
    protected void onStart(){
        super.onStart();

        startPeriodic();
    }

    private void makeRequest(){
        JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, server_url, null, new Response.Listener<JSONObject>() {
            @Override
            public void onResponse(JSONObject response) {
                Log.d("REQUEST", "request received");
                try{
                    ArrayList<DistanceReading> readingLis = new ArrayList<>();

                    readingLis.add(new DistanceReading(45, Float.valueOf(response.get("angle45").toString())));
                    readingLis.add(new DistanceReading(60, Float.valueOf(response.get("angle60").toString())));
                    readingLis.add(new DistanceReading(75, Float.valueOf(response.get("angle75").toString())));
                    readingLis.add(new DistanceReading(90, Float.valueOf(response.get("angle90").toString())));
                    readingLis.add(new DistanceReading(105, Float.valueOf(response.get("angle105").toString())));
                    readingLis.add(new DistanceReading(120, Float.valueOf(response.get("angle120").toString())));
                    readingLis.add(new DistanceReading(135, Float.valueOf(response.get("angle135").toString())));
                    display.drawDots();
                    display.invalidate();
                } catch (JSONException e) {
                    Log.e("ERROR", e.getMessage());
                }
            }
        }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                Log.e("VOLLEY ERROR", "Request could not be made");
            }
        });
        queue.add(jsonObjectRequest);
    }

    private void startPeriodic(){
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                makeRequest();
                handler.postDelayed(this, delayTime);
            }
        }, delayTime);
    }

    private void stopPeriodic(){
        handler.removeCallbacksAndMessages(null);
    }
}
