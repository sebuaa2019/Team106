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
import com.example.alarmapp.Utils.AppController;
import com.example.alarmapp.Utils.SensorInfo;

import org.json.JSONObject;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static com.example.alarmapp.Utils.URLConf.ONOFF;
import static com.example.alarmapp.Utils.URLConf.USING_URL;

public class MySensorRecycleViewAdapter extends RecyclerView.Adapter<MySensorRecycleViewAdapter.ContactViewHolder1> {

    private List<SensorInfo> sensorInfoList;

    public MySensorRecycleViewAdapter(List<SensorInfo> sensorInfoList){
        this.sensorInfoList = sensorInfoList;
    }

    @Override
    public ContactViewHolder1 onCreateViewHolder
            (ViewGroup parent, int viewType) {
        View itemView = LayoutInflater.from(parent.getContext()).
                //change to time line view
                        inflate(R.layout.card_view,parent,false);
        return new ContactViewHolder1(itemView);
    }

    @Override
    public void onBindViewHolder(final ContactViewHolder1 holder, final int position) {
        final SensorInfo si = sensorInfoList.get(position);
        holder.type.setText("类别："+si.getCategory());
        holder.pos.setText("位置: "+si.getPos());
        if(si.isOn()){
            holder.btn_on.setBackgroundColor(Color.rgb(244,67,54));
        }

        holder.btn_on.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Toast.makeText(holder.itemView.getContext(), "test"+String.valueOf(position),
                        Toast.LENGTH_LONG).show();
                try {
                    //
                    SharedPreferences sp = holder.itemView.getContext().getSharedPreferences("conf", 0);
                    final String token = sp.getString("token","");

                    String url = USING_URL + ONOFF;
                    final String tag = "json_onoff";
                    Map<String, Integer> map = new HashMap<String, Integer>();
                    int state = si.isOn() == true ? 0 : 1; // 如果此时按钮的状态是打开状态，设置为0，即关闭
                    map.put("on_off", state);
                    map.put("sensor_id", si.getSensor_id());
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

                    // 将替换掉的传感器信息重新设置
                    SensorInfo changeState = sensorInfoList.get(position);
                    changeState.setOn();
                    sensorInfoList.set(position, changeState);
                    notifyDataSetChanged();

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
        return sensorInfoList.size();
    }

    class ContactViewHolder1 extends RecyclerView.ViewHolder {
        //create the viewHolder class

        protected TextView type;
        protected TextView pos;
        protected Button btn_on;

        public ContactViewHolder1(View itemView) {
            super(itemView);
            type = itemView.findViewById(R.id.type);
            pos = itemView.findViewById(R.id.position);
            btn_on = itemView.findViewById(R.id.btn_on);
        }
    }
}
