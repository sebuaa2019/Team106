package com.example.alarmapp.Fragment;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.example.alarmapp.R;
import com.example.alarmapp.Utils.AppController;

import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

import static com.example.alarmapp.Utils.URLConf.*;

public class FragmentHome extends Fragment {
    private Spinner modeSpinner = null;
    private Spinner sensorSpinner = null;
    private ImageView iv = null;
    private TextView tv_name = null;
    private TextView tv_phone = null;
    private Button btn_onoff;
    private int mode=0; //记录开启模式
    private int sensor=0; //记录开启传感器
    private int state=1; //0表示关闭，1表示开启

    public FragmentHome(){

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fg_home, container, false);
        Log.i("Fragment1", "Home");
        iv = view.findViewById(R.id.user_image);
        tv_name = view.findViewById(R.id.user_name);
        tv_phone = view.findViewById(R.id.user_phone);
        modeSpinner = view.findViewById(R.id.h_mode_spinner);
        sensorSpinner = view.findViewById(R.id.h_sensor_spinner);
        btn_onoff = view.findViewById(R.id.btn_onoff);
        initEvent();
        getUserInfo();
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
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {
                sensor = 0;
            }
        });
        btn_onoff.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                switch (state){
                    case 0:
                        state = 1;
                        btn_onoff.setActivated(true);
                        break;
                    case 1:
                        state = 0;
                        btn_onoff.setActivated(false);
                        break;
                }
                on_off();
            }
        });
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
                                String img_path = response.getString("img_path");
                                tv_name.setText(name);
                                tv_phone.setText(phone);
                                //TODO: 头像的处理

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

    private void on_off(){
        try {
            //
            SharedPreferences sp = this.getActivity().getSharedPreferences("conf", 0);
            final String token = sp.getString("token","");

            String url = USING_URL + ONOFF;
            final String tag = "json_onoff";
            Map<String, Integer> map = new HashMap<String, Integer>();
            map.put("on_off", state);
            map.put("mode", mode);
            map.put("sensor", sensor);
            JSONObject params = new JSONObject(map);
            JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, params,
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            try{
                                if(response.getInt("status") == 0){
                                    Toast.makeText(getContext(), response.getString("msg"),
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
}
