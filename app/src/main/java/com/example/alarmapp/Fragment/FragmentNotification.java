package com.example.alarmapp.Fragment;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.TextView;
import android.widget.Toast;

import com.example.alarmapp.Adapter.MyRecycleViewAdapter;
import com.example.alarmapp.R;
import com.example.alarmapp.Utils.AlarmInfo;

import java.util.ArrayList;
import java.util.List;
import static com.example.alarmapp.Utils.URLConf.*;


public class FragmentNotification extends Fragment {
    private Button button = null;
    private DatePicker datePicker = null;
    private String text;
    private RecyclerView card_list;
    private MyRecycleViewAdapter myRecycleViewAdapter;
    private List<AlarmInfo> alarmInfoList = new ArrayList<>();

    public FragmentNotification(){

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fg_notification, container, false);
        Log.i("Fragment2", "notification");
        button = view.findViewById(R.id.select_date_button);
        card_list = view.findViewById(R.id.card_list);
        card_list.setHasFixedSize(true);
        LinearLayoutManager layoutManager = new LinearLayoutManager(getActivity());
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);
        card_list.setLayoutManager(layoutManager);
        generate_test_data();
        //TODO: 考虑刷新
        myRecycleViewAdapter = new MyRecycleViewAdapter(alarmInfoList);
        card_list.setAdapter(myRecycleViewAdapter);
//        card_list.setNestedScrollingEnabled(false);
        initEvent();
        return view;
    }

    private void generate_test_data(){
        AlarmInfo a1 = new AlarmInfo(1, "客厅", "烟感", "2019-05-14", false);
        alarmInfoList.add(a1);
        AlarmInfo a2 = new AlarmInfo(2, "客厅", "烟感", "2019-05-14", false);
        alarmInfoList.add(a2);
        AlarmInfo a3 = new AlarmInfo(3, "客厅", "烟感", "2019-05-14", true);
        alarmInfoList.add(a3);
        AlarmInfo a4 = new AlarmInfo(4, "客厅", "烟感", "2019-05-14", true);
        alarmInfoList.add(a4);
        AlarmInfo a5 = new AlarmInfo(5, "客厅", "烟感", "2019-05-14", false);
        alarmInfoList.add(a5);
        AlarmInfo a6 = new AlarmInfo(6, "客厅", "烟感", "2019-05-14", false);
        alarmInfoList.add(a6);
        AlarmInfo a7 = new AlarmInfo(7, "客厅", "烟感", "2019-05-14", true);
        alarmInfoList.add(a7);
        AlarmInfo a8 = new AlarmInfo(8, "客厅", "烟感", "2019-05-14", false);
        alarmInfoList.add(a8);
        AlarmInfo a9 = new AlarmInfo(9, "客厅", "烟感", "2019-05-14", true);
        alarmInfoList.add(a9);

    }

    private void initEvent(){
        //TODO: 设置一旦spinner的值变动或者日期变动，即调用接口更新
        button.setOnClickListener(new View.OnClickListener(){
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
        datePicker.init(2019, 5, 9, new DatePicker.OnDateChangedListener() {
            @Override
            public void onDateChanged(DatePicker datePicker, int i, int i1, int i2) {
                text = "Year:"+i+" Month:"+i1+" Day:"+i2 ;
            }
        });

        builder.setPositiveButton(R.string.confirm, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                //设置时间
                Toast.makeText(getContext(), text, Toast.LENGTH_LONG).show();
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
}
