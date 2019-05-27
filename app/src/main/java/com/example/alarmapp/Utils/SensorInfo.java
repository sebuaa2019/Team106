package com.example.alarmapp.Utils;

import java.util.ArrayList;

public class SensorInfo {
    private int sensor_id;
    private int category;
    private String pos;
    private boolean on; // true 表示处于打开状态，false 表示处于关闭状态

    public SensorInfo(int sensor_id, int category, String pos, boolean on){
        this.sensor_id = sensor_id;
        this.category = category;
        this.pos = pos;
        this.on = on;
    }

    public int getSensor_id() {
        return sensor_id;
    }

    public String getPos() {
        return pos;
    }

    public boolean isOn() {
        return on;
    }

    public String getCategory() {
        String [] sensor_list = {"门磁","红外", "水浸", "烟感","温感"};
        return sensor_list[category];
    }
    public int getCategoryId(){
        return category;
    }

    public void setOn(){
        this.on = ! this.isOn();
    }
}
