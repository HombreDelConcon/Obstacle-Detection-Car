package com.ongolly.smokerv2interface;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;
import java.lang.Math.*;
import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;
import java.util.Vector;

public class SensorOutput extends View {
    private final float conversionScalar = 825/145;
    private Paint dot;
    private Canvas canv;
    private int radius = 30;
    private float viewWidth;
    private float viewHeight;
    private double pi = Math.PI;

    //Store X,Y coordinates in sub ArrayLists. Why didn't I just create a wrapper object?
    //  I don't know. Maybe I'm tired of the infinite layers of abstraction brought
    //  forth in Java.
    private ArrayList<ArrayList<Float>> coords;

    public SensorOutput(Context context) {
        super(context);
        init(context);
    }

    public SensorOutput(Context context, AttributeSet attrs){
        super(context, attrs);
        init(context);
    }
    public SensorOutput(Context context, AttributeSet attrs, int defStyle){
        super(context, attrs, defStyle);
        init(context);
    }

    public void init(Context context){
        dot = new Paint(Paint.ANTI_ALIAS_FLAG);
        dot.setStyle(Paint.Style.FILL);
        dot.setColor(Color.YELLOW);
        dot.setTextSize(50);

        coords = new ArrayList<ArrayList<Float>>();
    }

    @Override
    protected void onDraw(Canvas canvas){
        super.onDraw(canvas);
        canv = canvas;
        canvas.drawColor(Color.BLACK);
        drawDots();
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldW, int oldH){
        viewWidth = w;
        viewHeight = h;
    }

    //Take every ArrayList in the coords variable and get the X and Y coordinates
    //  stored inside.
    public void drawDots(){
        for (ArrayList<Float> sub : coords) {
            canv.drawCircle((viewWidth / 2) - (sub.get(0)*conversionScalar), (viewHeight) - (sub.get(1)*conversionScalar), radius, dot);
            invalidate();
        }
    }

    //Take a DistanceReading object and use some simple trig to determine
    //  the X and Y coordinates of this
    private ArrayList<Float> calcXY(DistanceReading reading){
        ArrayList<Float> returnLis = new ArrayList<Float>();
        Float x = Float.valueOf((float) (reading.getLength() * Math.cos(reading.getAngle())));
        Float y = Float.valueOf((float) (reading.getLength() * Math.sin(reading.getAngle())));

        returnLis.add(x);
        returnLis.add(y);

        return returnLis;
    }
    //Take a list of DistanceReading objects and process them all to
    //  get the X and Y coordinates of the where that point should
    //  fall in the View. These are stored as a pair in an ArrayList
    //  which is added to the coords list.
    public void addReadings(ArrayList<DistanceReading> lis){
        for (DistanceReading d : lis){
            this.coords.add(calcXY(d));
        }
    }
}