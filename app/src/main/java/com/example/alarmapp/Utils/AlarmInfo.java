package com.example.alarmapp.Utils;

public class AlarmInfo{
    private int record_id;
    private String pos;
    private String type;
    private int time;
    private boolean is_alarm; //字段用来表示是否可以点击 Button解除警报，并且设置Button的

    public AlarmInfo(int record_id, String pos, String type, int time, boolean is_alarm){
        this.record_id = record_id;
        this.pos = pos;
        this.type = type;
        this.time = time;
        this.is_alarm = is_alarm;
    }

    public String getPos() {
        return pos;
    }

    public String getType() {
        return type;
    }

    public int getTime() {
        return time;
    }

    public boolean isIs_alarm() {
        return is_alarm;
    }

    public int getRecord_id() {
        return record_id;
    }
}
