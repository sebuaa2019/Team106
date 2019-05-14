package com.example.alarmapp.Adapter;

import android.graphics.Color;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;

import com.example.alarmapp.R;
import com.example.alarmapp.Utils.AlarmInfo;

import java.util.List;

public class MyRecycleViewAdapter extends RecyclerView.Adapter<MyRecycleViewAdapter.ContactViewHolder> {

    private List<AlarmInfo> alarmInfoList;

    public MyRecycleViewAdapter(List<AlarmInfo> alarmInfoList){
        this.alarmInfoList = alarmInfoList;
    }

    @Override
    public ContactViewHolder onCreateViewHolder
            (ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(parent.getContext()).
                inflate(R.layout.card_view,parent,false);
        return new ContactViewHolder(itemView);
    }

    @Override
    public void onBindViewHolder(ContactViewHolder holder, int position) {
        AlarmInfo ai = alarmInfoList.get(position);
        holder.pos.setText("位置："+ai.getPos());
        holder.time.setText("时间："+ai.getTime());
        holder.type.setText("类别："+ai.getType());
        if(ai.isIs_alarm()){
            //处于警戒状态
            //TODO: 换他妈的样式
            holder.cancel.setBackgroundColor(Color.rgb(255,97,0));
        }
    }

    @Override
    public int getItemCount() {
        return alarmInfoList.size();
    }

    class ContactViewHolder extends RecyclerView.ViewHolder {
        //create the viewHolder class

        protected TextView pos;
        protected TextView time;
        protected TextView type;
        protected Button cancel;

        public ContactViewHolder(View itemView) {
            super(itemView);
            pos = itemView.findViewById(R.id.position);
            time = itemView.findViewById(R.id.time);
            type = itemView.findViewById(R.id.type);
            cancel = itemView.findViewById(R.id.btn_cancel);
        }
    }
}
