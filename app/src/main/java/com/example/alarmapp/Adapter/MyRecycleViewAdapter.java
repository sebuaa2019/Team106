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
        holder.pos.setText("位置："+ai.getPos());
        holder.time.setText("时间："+ai.getTime());
        holder.type.setText("类别："+ai.getType());
        Log.d("test btn_cancel", String.valueOf(ai.isIs_alarm()) + String.valueOf(position));
        if(ai.isIs_alarm()){
            //处于警戒状态
            //TODO: 这里有个bug，不是true状态的也是红色，，不知道为什么
            //change alarm button color
            holder.cancel.setBackgroundColor(Color.rgb(244,67,54));
            //add time line
            if (position == 0) {
                //delete top line
                holder.tv_line_top.setVisibility(View.INVISIBLE);
            } else if (position == alarmInfoList.size() - 1) {
                //delete bottom line
                holder.tv_line_bottom.setVisibility(View.INVISIBLE);
            } else {
                //set dot
                holder.tv_dot.setBackgroundResource(R.drawable.time_line_dot);
            }
        }
        holder.cancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Log.d("btn_cancel", String.valueOf(position));
                try {
                    //
                    SharedPreferences sp = holder.itemView.getContext().getSharedPreferences("conf", 0);
                    final String token = sp.getString("token","");

                    String url = USING_URL + CANCELALARM;
                    final String tag = "json_cancel_alarm";
                    Map<String, Integer> map = new HashMap<String, Integer>();
                    map.put("record_id", alarmInfoList.get(position).getRecord_id());
                    JSONObject params = new JSONObject(map);
                    JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, params,
                            new Response.Listener<JSONObject>() {
                                @Override
                                public void onResponse(JSONObject response) {
                                    try{
                                        if(response.getInt("status") == 0){
                                            Toast.makeText(holder.itemView.getContext(), response.getString("msg"),
                                                    Toast.LENGTH_LONG).show();
                                        }
                                    }catch (Exception e){
                                        e.printStackTrace();
                                    }
                                }
                            },
                            new Response.ErrorListener() {
                                @Override
                                public void onErrorResponse(VolleyError error) {
                                    Log.e(tag, error.toString());
                                }
                            }){
                        @Override
                        public Map<String, String> getHeaders() throws AuthFailureError {
                            HashMap<String, String> headers = new HashMap<String, String>();
                            headers.put("Content-Type", "application/json");
                            headers.put("token", token);
                            return headers;
                        }
                    };
                    AppController.getInstance().addToRequestQueue(jsonObjectRequest, tag);

                }catch (NullPointerException e){
                    e.printStackTrace();
                }catch (Exception e){
                    e.printStackTrace();
                }
            }
        });
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
        private final TextView tv_line_top;
        private final TextView tv_line_bottom;
        private final TextView tv_dot;

        public ContactViewHolder(View itemView) {
            super(itemView);
            pos = itemView.findViewById(R.id.position);
            time = itemView.findViewById(R.id.time);
            type = itemView.findViewById(R.id.type);
            cancel = itemView.findViewById(R.id.btn_cancel);

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
