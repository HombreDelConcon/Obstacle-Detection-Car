package com.ongolly.smokerv2interface;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.RequestQueue;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONException;
import org.json.JSONObject;

public class RobotControlActivity extends AppCompatActivity{
    final private String server_url = "http://192.168.62.31:5000/smokerV2";//"http://10.0.0.7:5000/smokerV2";

    private Button forward;
    private Button back;
    private Button left;
    private Button right;
    private Button stop;
    private Button l_speed_up;
    private Button l_speed_down;
    private Button r_speed_up;
    private Button r_speed_down;
    private TextView l_speed_text;
    private TextView r_speed_text;
    private RequestQueue queue;
    private int l_speed = 1;
    private int r_speed = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.smoker_control_layout);

        queue = Volley.newRequestQueue(RobotControlActivity.this);
        forward = (Button) findViewById(R.id.up);
        back = (Button) findViewById(R.id.down);
        left = (Button) findViewById(R.id.left);
        right = (Button) findViewById(R.id.right);
        stop = (Button) findViewById(R.id.stop);
        l_speed_up = (Button) findViewById(R.id.l_speed_up);
        l_speed_down = (Button) (Button) findViewById(R.id.l_speed_down);
        r_speed_up = (Button) findViewById(R.id.r_speed_up);
        r_speed_down = (Button) findViewById(R.id.r_speed_down);
        l_speed_text = (TextView) findViewById(R.id.l_speed_view);
        r_speed_text = (TextView) findViewById(R.id.r_speed_view);

        l_speed_up.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (l_speed < 10){
                    l_speed++;
                }
                l_speed_text.setText(String.valueOf(l_speed));
            }
        });

        l_speed_down.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (l_speed > 1) {
                    l_speed--;
                }
                l_speed_text.setText(String.valueOf(l_speed));
            }
        });

        r_speed_up.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (r_speed < 10){
                    r_speed++;
                }
                r_speed_text.setText(String.valueOf(r_speed));
            }
        });

        r_speed_down.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (r_speed > 1) {
                    r_speed--;
                }
                r_speed_text.setText(String.valueOf(r_speed));
            }
        });

        forward.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                JSONObject json = new JSONObject();
                try {
                    json.append("lmotor", 1);
                    json.append("rmotor", 1);
                    json.append("lspeed", l_speed);
                    json.append("rspeed", r_speed);
                    json.append("type", 2);
                } catch (JSONException e) {
                    e.printStackTrace();
                }
                JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, server_url, json, new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        Log.d("JSON REQUEST", "Response received");
                    }
                }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.d("VOLLEY ERROR", error.getMessage());
                    }
                });
                queue.add(jsonObjectRequest);
            }
        });

        stop.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                JSONObject json = new JSONObject();
                try {
                    json.append("lmotor", 0);
                    json.append("rmotor", 0);
                    json.append("lspeed", l_speed);
                    json.append("rspeed", r_speed);
                    json.append("type", 2);
                } catch (JSONException e) {
                    e.printStackTrace();
                }
                JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, server_url, json, new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        Log.d("JSON REQUEST", "Response received");
                    }
                }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.d("VOLLEY ERROR", error.getMessage());
                    }
                });
                queue.add(jsonObjectRequest);
            }
        });

        back.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                JSONObject json = new JSONObject();
                try {
                    json.append("lmotor", -1);
                    json.append("rmotor", -1);
                    json.append("lspeed", l_speed);
                    json.append("rspeed", r_speed);
                    json.append("type", 2);
                } catch (JSONException e) {
                    e.printStackTrace();
                }
                JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, server_url, json, new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        Log.d("JSON REQUEST", "Response received");
                    }
                }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.d("VOLLEY ERROR", error.getMessage());
                    }
                });
                queue.add(jsonObjectRequest);
            }
        });

        left.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                JSONObject json = new JSONObject();
                try {
                    json.append("lmotor", -1);
                    json.append("rmotor", 1);
                    json.append("lspeed", l_speed);
                    json.append("rspeed", r_speed);
                    json.append("type", 2);
                } catch (JSONException e) {
                    e.printStackTrace();
                }
                JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, server_url, json, new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        Log.d("JSON REQUEST", "Response received");
                    }
                }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.d("VOLLEY ERROR", error.getMessage());
                    }
                });
                queue.add(jsonObjectRequest);
            }
        });

        right.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                JSONObject json = new JSONObject();
                try {
                    json.append("lmotor", 1);
                    json.append("rmotor", -1);
                    json.append("lspeed", l_speed);
                    json.append("rspeed", r_speed);
                    json.append("type", 2);
                } catch (JSONException e) {
                    e.printStackTrace();
                }
                JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, server_url, json, new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        Log.d("JSON REQUEST", "Response received");
                    }
                }, new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        Log.d("VOLLEY ERROR", error.getMessage());
                    }
                });
                queue.add(jsonObjectRequest);
            }
        });
    }
}
