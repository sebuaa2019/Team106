package com.example.alarmapp.Utils;

public class AlarmInfo{
    private int record_id;
    private String type;
    private int time;

    public AlarmInfo(int record_id, String type, int time){
        this.record_id = record_id;
        this.type = type;
        this.time = time;
    }


    public String getType() {
        return type;
    }

    public int getTime() {
        return time;
    }

    public int getRecord_id() {
        return record_id;
    }
}
