package com.example.alarmapp.Fragment;

import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.example.alarmapp.R;

public class FragmentHome extends Fragment {

    public FragmentHome(){

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fg_home, container, false);
        Log.i("Fragment1", "Home");
        return view;
    }
}
