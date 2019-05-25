package com.example.alarmapp.Fragment;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.content.DialogInterface;
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
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.example.alarmapp.Adapter.MyRecycleViewAdapter;
import com.example.alarmapp.R;
import com.example.alarmapp.Utils.AlarmInfo;
import com.example.alarmapp.Utils.AppController;

import org.json.JSONArray;
import org.json.JSONObject;

import java.util.Calendar;
import java.util.Date;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.GregorianCalendar;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static com.example.alarmapp.R.color;
import static com.example.alarmapp.Utils.URLConf.*;


public class FragmentNotification extends Fragment {
    private Spinner spin_sensor = null;
    private Button btn_select_date = null;
    private DatePicker datePicker = null;
    private String date_text="";
    private int sensor = 0;

    private RecyclerView card_list;
    private MyRecycleViewAdapter myRecycleViewAdapter;
    private List<AlarmInfo> alarmInfoList = new ArrayList<>();

    public FragmentNotification(){

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fg_notification, container, false);
        Log.i("Fragment2", "notification");
        spin_sensor = view.findViewById(R.id.n_sensor_spinner);
        btn_select_date = view.findViewById(R.id.select_date_button);
        card_list = view.findViewById(R.id.card_list);
        card_list.setHasFixedSize(true);
        LinearLayoutManager layoutManager = new LinearLayoutManager(getActivity());
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        card_list.setLayoutManager(layoutManager);
        generate_test_data();
        sortByPriority();
        //TODO: 考虑刷新
        myRecycleViewAdapter = new MyRecycleViewAdapter(alarmInfoList);
        card_list.setAdapter(myRecycleViewAdapter);
//        card_list.setNestedScrollingEnabled(false);
        initEvent();
        return view;
    }



    private void initEvent(){
        spin_sensor.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @SuppressLint("ResourceAsColor")
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                TextView tv=(TextView)view;
                tv.setTextColor(R.color.colorPrimary);
                sensor = i;
                queryIndex();
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {
                sensor = 0;
            }
        });


        btn_select_date.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view){
                showDialog();
            }
        });
    }

    private void showDialog(){
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflater = getActivity().getLayoutInflater();
        View view = inflater.inflate(R.layout.datepicker_layout, null);
        builder.setView(view);
        /*
        * 这里一定要搞清楚，datepicker是在弹出dialog之后才加载，所以获取datepicker对象也要在其存在的情况下，】
        * 否则会出现NullPointerException
        * */
        datePicker = view.findViewById(R.id.date_picker);
        Calendar cal = new GregorianCalendar();

        dateRegulation(cal.get(Calendar.YEAR), cal.get(Calendar.MONTH), cal.get(Calendar.DAY_OF_MONTH));
        datePicker.init(cal.get(Calendar.YEAR), cal.get(Calendar.MONTH), cal.get(Calendar.DAY_OF_MONTH),
                new DatePicker.OnDateChangedListener() {
            @Override
            public void onDateChanged(DatePicker datePicker, int i, int i1, int i2) {
                dateRegulation(i,i1,i2);
            }
        });

        builder.setPositiveButton(R.string.confirm, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                //设置时间
                Toast.makeText(getContext(), date_text, Toast.LENGTH_LONG).show();
                queryIndex();
            }
        });
        builder.setNegativeButton(R.string.cancel, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                //取消
            }
        });
        AlertDialog dialog = builder.create();
        dialog.show();
    }

    private void queryIndex(){
        try {
            //构建位置，传感器的映射
            final ArrayList<String> sensor_map = new ArrayList<String>();
            sensor_map.add("门磁");
            sensor_map.add("水浸");
            sensor_map.add("红外");
            sensor_map.add("烟雾");
            sensor_map.add("温度");

            // 掉接口
            SharedPreferences sp = this.getActivity().getSharedPreferences("conf", 0);
            final String token = sp.getString("token","");

            String url = USING_URL + RECORD;
            final String tag = "json_query_index";
            Map<String, String> map = new HashMap<String, String>();
            map.put("sensor", String.valueOf(sensor));
            map.put("date", date_text);
            JSONObject params = new JSONObject(map);
            JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, params,
                    new Response.Listener<JSONObject>() {
                        @Override
                        public void onResponse(JSONObject response) {
                            try{
                                int total = response.getInt("count");
                                JSONArray items = response.getJSONArray("info_list");
                                alarmInfoList.clear();
                                for(int i=0; i< items.length(); i++){
                                    JSONObject item = items.getJSONObject(i);
                                    int record_id = item.getInt("record_id");
                                    String type = sensor_map.get(item.getInt("category"));
                                    String pos = item.getString("pos");
                                    //考虑将时间转换为int,精确到秒
                                    long time = item.getLong("time");
                                    AlarmInfo ai = new AlarmInfo(record_id, type, pos, time);
                                    alarmInfoList.add(ai);
                                }
                                //排序，先按照is_alarm, 再按照时间
                                sortByPriority();
                                myRecycleViewAdapter.notifyDataSetChanged();
                                //需要测试是否会刷新
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
                    headers.put("Authorization", "Bearer "+token);
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
        AlarmInfo a1 = new AlarmInfo(1,  "烟感", "客厅",1111111);
        alarmInfoList.add(a1);
        AlarmInfo a2 = new AlarmInfo(2,  "烟感", "客厅",22211111);
        alarmInfoList.add(a2);
        AlarmInfo a3 = new AlarmInfo(3, "烟感", "客厅",112111111);
        alarmInfoList.add(a3);
        AlarmInfo a4 = new AlarmInfo(4,"烟感", "客厅",221111111);
        alarmInfoList.add(a4);
        AlarmInfo a5 = new AlarmInfo(5,"烟感", "客厅",33311111);
        alarmInfoList.add(a5);
        AlarmInfo a6 = new AlarmInfo(6,  "烟感", "客厅",444111111);
        alarmInfoList.add(a6);
        AlarmInfo a7 = new AlarmInfo(7,  "烟感", "客厅",55511111);
        alarmInfoList.add(a7);
        AlarmInfo a8 = new AlarmInfo(8,  "烟感", "客厅",11111111);
        alarmInfoList.add(a8);
        AlarmInfo a9 = new AlarmInfo(9, "烟感", "客厅",23311111);
        alarmInfoList.add(a9);

    }

    private void sortByPriority(){
        /*
        * 这里的排序算法按照时间
        * */
        //把报警分为紧急和已处理两类
        Collections.sort(alarmInfoList, new Comparator<AlarmInfo>() {
            @Override
            public int compare(AlarmInfo alarmInfo, AlarmInfo t1) {
                Long time = alarmInfo.getTime();
                Long time1 = t1.getTime();
                return time1.compareTo(time);
            }
        });
        for(int i=0;i<alarmInfoList.size();i++){
            Log.d("11", String.valueOf(String.valueOf(alarmInfoList.get(i).getTime())));
        }
    }


    private void dateRegulation(int year, int month, int day){
        //month 是从0开始计算的
        month ++;
        if(month <= 9)
            date_text = year + "-0" + month;
        else
            date_text = year + "-"+ month;

        if(day <= 9)
            date_text += "-0"+day;
        else
            date_text += "-"+day;
    }
}
