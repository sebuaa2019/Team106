package com.example.alarmapp.Fragment;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.DatePicker;
import android.widget.TextView;
import android.widget.Toast;

import com.example.alarmapp.R;

public class FragmentNotification extends Fragment {
    private Button button = null;
    private DatePicker datePicker = null;
    private TextView date_text = null;
    private String text;

    public FragmentNotification(){

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fg_notification, container, false);
        Log.i("Fragment2", "notification");
        button = view.findViewById(R.id.select_date_button);
        date_text = view.findViewById(R.id.date_text);
        initEvent();
        return view;
    }

    private void initEvent(){
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
                Toast.makeText(getContext(), "xiajiba", Toast.LENGTH_LONG).show();
                date_text.setText(text);
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
