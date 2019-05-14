package com.example.alarmapp.Fragment;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.ToggleButton;

import com.example.alarmapp.R;

public class FragmentHome extends Fragment {
    private Spinner modeSpinner = null;
    private Spinner posSpinner = null;
    private TextView name;
    private TextView phone;
    private ToggleButton toggleButton;
    private int mode=0; //记录开启模式
    private int pos=0; //记录开启位置

    public FragmentHome(){

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fg_home, container, false);
        Log.i("Fragment1", "Home");
        modeSpinner = view.findViewById(R.id.h_mode_spinner);
        posSpinner = view.findViewById(R.id.h_pos_spinner);
        initEvent();
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

            }
        });
        posSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                pos = i;
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {

            }
        });
    }
}
