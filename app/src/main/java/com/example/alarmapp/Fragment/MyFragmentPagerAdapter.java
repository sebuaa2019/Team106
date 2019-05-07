package com.example.alarmapp.Fragment;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentPagerAdapter;
import android.view.ViewGroup;

import com.example.alarmapp.MainActivity;

public class MyFragmentPagerAdapter extends FragmentPagerAdapter {

    private final int PAGER_COUNT = 3;
    private FragmentHome fragmentHome = null;
    private FragmentNotification fragmentNotification = null;
    private FragmentSettings fragmentSettings = null;

    public MyFragmentPagerAdapter(FragmentManager fm){
        super(fm);
        fragmentHome = new FragmentHome();
        fragmentNotification = new FragmentNotification();
        fragmentSettings = new FragmentSettings();
    }

    @Override
    public int getCount(){
        return PAGER_COUNT;
    }

    @Override
    public Object instantiateItem(ViewGroup vg, int position) {
        return super.instantiateItem(vg, position);
    }

    @Override
    public void destroyItem(ViewGroup container, int position, Object object) {
        System.out.println("position Destory" + position);
        super.destroyItem(container, position, object);
    }

    @Override
    public Fragment getItem(int position){
        Fragment fragment = null;
        switch (position){
            case MainActivity.PAGE_HOME:
                fragment = fragmentHome;
                break;
            case MainActivity.PAGE_NOTIFICATION:
                fragment = fragmentNotification;
                break;
            case MainActivity.PAGE_SETTINGS:
                fragment = fragmentSettings;
                break;
        }
        return fragment;
    }

}
