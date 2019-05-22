package com.example.alarmapp.Utils;

public class AlarmInfo{
    private int record_id;
    private String type;
    private int time;
    private String pos;

    public AlarmInfo(int record_id, String type, String pos, int time){
        this.record_id = record_id;
        this.type = type;
        this.time = time;
        this.pos = pos;
    }


    public String getType() {
        return type;
    }

    public int getTime() {
        return time;
    }

    public String getPos() {
        return pos;
    }
}
