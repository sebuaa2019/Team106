package com.example.alarmapp.Fragment;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.alarmapp.R;

public class FragmentNotification extends Fragment {
    public FragmentNotification(){

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fg_notification, container, false);
        Log.i("Fragment2", "notification");
        return view;
    }

}
