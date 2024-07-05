package com.ongolly.smokerv2interface;

//Class used to store all the readings to make it easier to pass them
//  from screen to screen. It stores the angle and measured distance
//  for each reading.
public class DistanceReading {
    private final double angle;
    private final float length;

    public DistanceReading(double angle, float length){
        this.angle = angle;
        this.length = length;
    }

    public double getAngle(){
        return this.angle;
    }
    public float getLength(){
        return this.length;
    }
}
