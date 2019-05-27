package com.example.alarmapp.Adapter;

import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import com.example.alarmapp.R;
import com.example.alarmapp.Utils.AlarmInfo;

import java.text.SimpleDateFormat;
import java.util.List;
import java.util.Date;

public class MyRecycleViewAdapter extends RecyclerView.Adapter<MyRecycleViewAdapter.ContactViewHolder> {

    private List<AlarmInfo> alarmInfoList;

    public MyRecycleViewAdapter(List<AlarmInfo> alarmInfoList){
        this.alarmInfoList = alarmInfoList;
    }

    @Override
    public ContactViewHolder onCreateViewHolder
            (ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(parent.getContext()).
                //change to time line view
                inflate(R.layout.time_line,parent,false);
        return new ContactViewHolder(itemView);
    }

    @Override
    public void onBindViewHolder(final ContactViewHolder holder, final int position) {
        AlarmInfo ai = alarmInfoList.get(position);
        Log.d("test", String.valueOf(ai.getTime()));
        Date date = new Date(ai.getTime()*1000);
        SimpleDateFormat ft = new SimpleDateFormat("yyyy-MM-dd hh:mm:ss");
        holder.time.setText("时间："+ft.format(date));
        holder.type.setText("类别："+ai.getType());
        holder.pos.setText("位置: "+ai.getPos());
    }

    @Override
    public int getItemCount() {
        return alarmInfoList.size();
    }

    class ContactViewHolder extends RecyclerView.ViewHolder {
        //create the viewHolder class

        protected TextView time;
        protected TextView type;
        protected TextView pos;
        private final TextView tv_line_top;
        private final TextView tv_line_bottom;
        private final TextView tv_dot;

        public ContactViewHolder(View itemView) {
            super(itemView);
            time = itemView.findViewById(R.id.time);
            type = itemView.findViewById(R.id.type);
            pos = itemView.findViewById(R.id.position);
            //TODO: add description for every type of alarm signal maybe
            //tv_desc = (TextView) itemView.findViewById(R.id.tv_desc);
            //top line vertical
            tv_line_top = (TextView) itemView.findViewById(R.id.tv_line_top);
            //dot
            tv_dot = (TextView) itemView.findViewById(R.id.tv_dot);
            //bottom line vertical
            tv_line_bottom = (TextView) itemView.findViewById(R.id.tv_line_bottom);

        }
    }
}
