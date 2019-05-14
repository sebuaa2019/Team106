package com.example.alarmapp.Adapter;

import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
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
        holder.position.setText("postion: "+ai.getPosition());
    }

    @Override
    public int getItemCount() {
        return alarmInfoList.size();
    }

    class ContactViewHolder extends RecyclerView.ViewHolder {
        //create the viewHolder class

        protected TextView position;

        public ContactViewHolder(View itemView) {
            super(itemView);
            position = itemView.findViewById(R.id.position);
        }
    }
}
