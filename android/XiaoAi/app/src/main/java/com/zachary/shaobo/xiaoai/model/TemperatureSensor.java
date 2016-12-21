package com.zachary.shaobo.xiaoai.model;

/**
 * Created by shaobo on 2016/12/21.
 */

public class TemperatureSensor extends Sensor{
    private String sensorType = "temperature";
    private float mTemp = 0;
    public float getTemp(){
        return mTemp;
    }

    @Override
    public String getSensorType(){
        return sensorType;
    }
}
