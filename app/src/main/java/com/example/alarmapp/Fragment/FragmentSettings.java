package com.example.alarmapp.Fragment;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.alarmapp.R;

public class FragmentSettings extends Fragment {

    public FragmentSettings(){

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fg_settings, container, false);
        Log.i("Fragment3", "settings");
        return view;
    }

}
