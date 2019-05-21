package com.example.alarmapp.Adapter;

import android.content.SharedPreferences;
import android.graphics.Color;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.example.alarmapp.R;
import com.example.alarmapp.Utils.AlarmInfo;
import com.example.alarmapp.Utils.AppController;

import org.json.JSONObject;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static com.example.alarmapp.Utils.URLConf.*;

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
        holder.time.setText("时间："+ai.getTime());
        holder.type.setText("类别："+ai.getType());
    }

    @Override
    public int getItemCount() {
        return alarmInfoList.size();
    }

    class ContactViewHolder extends RecyclerView.ViewHolder {
        //create the viewHolder class

        protected TextView time;
        protected TextView type;
        private final TextView tv_line_top;
        private final TextView tv_line_bottom;
        private final TextView tv_dot;

        public ContactViewHolder(View itemView) {
            super(itemView);
            time = itemView.findViewById(R.id.time);
            type = itemView.findViewById(R.id.type);

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
