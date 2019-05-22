package com.example.alarmapp.Utils;

public class AlarmInfo{
    private int record_id;
    private String type;
    private long time;
    private String pos;

    public AlarmInfo(int record_id, String type, String pos, long time){
        this.record_id = record_id;
        this.type = type;
        this.time = time;
        this.pos = pos;
    }


    public String getType() {
        return type;
    }

    public long getTime() {
        return time;
    }

    public String getPos() {
        return pos;
    }
}
