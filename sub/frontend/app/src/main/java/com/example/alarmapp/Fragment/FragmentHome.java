package com.example.alarmapp.Fragment;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.example.alarmapp.Adapter.MySensorRecycleViewAdapter;
import com.example.alarmapp.R;
import com.example.alarmapp.Utils.AppController;
import com.example.alarmapp.Utils.SensorInfo;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import static com.example.alarmapp.Utils.URLConf.*;

public class FragmentHome extends Fragment {
    private Spinner modeSpinner = null;
    private Spinner sensorSpinner = null;
    private TextView tv_name = null;
    private TextView tv_phone = null;
    private int mode=0; //记录开启模式
    private int sensor=0;

    private RecyclerView card_list;
    private MySensorRecycleViewAdapter mySensorRecycleViewAdapter;
    private ArrayList<SensorInfo> sensorInfoArrayList = new ArrayList<>();


    public FragmentHome(){

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fg_home, container, false);
        Log.i("Fragment1", "Home");
        tv_name = view.findViewById(R.id.user_name);
        tv_phone = view.findViewById(R.id.user_phone);
        modeSpinner = view.findViewById(R.id.h_mode_spinner);
        sensorSpinner = view.findViewById(R.id.h_sensor_spinner);

        card_list = view.findViewById(R.id.sensor_list);
        card_list.setHasFixedSize(true);
        LinearLayoutManager layoutManager = new LinearLayoutManager(getActivity());
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        card_list.setLayoutManager(layoutManager);

        mySensorRecycleViewAdapter = new MySensorRecycleViewAdapter(sensorInfoArrayList);
        card_list.setAdapter(mySensorRecycleViewAdapter);

        initEvent();
        getUserInfo();
        getSensorState();
        return view;
    }

    private void initEvent(){
        modeSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                mode = i;
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {
                mode = 0; //设置一个默认值
            }
        });
        sensorSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                sensor = i;
                getSensorState();
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {
                sensor = 0;
            }
        });
        generate_test_data();
    }


    private void getUserInfo(){
        try {
            //
            SharedPreferences sp = this.getActivity().getSharedPreferences("conf", 0);
            final String token = sp.getString("token","");

            String url = USING_URL + INFOINDEX;
            final String tag = "json_info_index";
            JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.GET, url, null,
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            try{
                                String name = response.getString("name");
                                String phone = response.getString("phone");
                                tv_name.setText(name);
                                tv_phone.setText(phone);
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



    private void getSensorState(){
        try {
            //
            SharedPreferences sp = this.getActivity().getSharedPreferences("conf", 0);
            final String token = sp.getString("token","");

            String url = USING_URL + GETSENSOR;
            final String tag = "json_getsensorstate";
            Map<String, Integer> map = new HashMap<String, Integer>();
            map.put("type", sensor);
            JSONObject params = new JSONObject(map);
            JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, params,
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            try{
                                int total = response.getInt("count");
                                JSONArray items = response.getJSONArray("sensor_list");
                                sensorInfoArrayList.clear();
                                for (int i=0; i < items.length(); i++){
                                    JSONObject item = items.getJSONObject(i);
                                    int sensor_id = item.getInt("sensor_id");
                                    int type = item.getInt("type");
                                    String position = item.getString("pos");
                                    boolean on = item.getInt("status") == 1;
                                    SensorInfo si = new SensorInfo(sensor_id, type, position, on);
                                    sensorInfoArrayList.add(si);
                                }
                                mySensorRecycleViewAdapter.notifyDataSetChanged();
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

    private void generate_test_data(){
        SensorInfo si1 = new SensorInfo(0, 0, "大门", true);
        sensorInfoArrayList.add(si1);
        SensorInfo si2 = new SensorInfo(1, 1, "厨房", false);
        sensorInfoArrayList.add(si2);
        SensorInfo si3 = new SensorInfo(2, 2, "卧室", false);
        sensorInfoArrayList.add(si3);
        SensorInfo si4 = new SensorInfo(3, 3, "书房", true);
        sensorInfoArrayList.add(si4);
        SensorInfo si5 = new SensorInfo(4, 4, "厨房", true);
        sensorInfoArrayList.add(si5);
    }

}
