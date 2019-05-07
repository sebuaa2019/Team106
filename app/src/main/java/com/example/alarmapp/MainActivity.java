package com.example.alarmapp;

import android.os.Bundle;
import android.support.design.widget.BottomNavigationView;
import android.support.v4.view.ViewPager;
import android.support.v7.app.AppCompatActivity;
import android.support.annotation.NonNull;
import android.view.MenuItem;

import com.example.alarmapp.Fragment.MyFragmentPagerAdapter;

public class MainActivity extends AppCompatActivity implements ViewPager.OnPageChangeListener {
    public static final int PAGE_HOME = 0;
    public static final int PAGE_NOTIFICATION = 1;
    public static final int PAGE_SETTINGS = 2;

    private ViewPager viewPager;
    private MyFragmentPagerAdapter myFragmentPagerAdapter;
    private BottomNavigationView navView;

    private BottomNavigationView.OnNavigationItemSelectedListener mOnNavigationItemSelectedListener
            = new BottomNavigationView.OnNavigationItemSelectedListener() {

        @Override
        public boolean onNavigationItemSelected(@NonNull MenuItem item) {
            switch (item.getItemId()) {
                case R.id.navigation_home:
                    viewPager.setCurrentItem(PAGE_HOME);
                    return true;
                case R.id.navigation_notifications:
                    viewPager.setCurrentItem(PAGE_NOTIFICATION);
                    return true;
                case R.id.navigation_settings:
                    viewPager.setCurrentItem(PAGE_SETTINGS);
                    return true;
            }
            return false;
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.home_page);
        navView = findViewById(R.id.nav_view);
        myFragmentPagerAdapter = new MyFragmentPagerAdapter(getSupportFragmentManager());
        viewPager = findViewById(R.id.vpager);
        viewPager.setAdapter(myFragmentPagerAdapter);
        viewPager.setCurrentItem(PAGE_HOME);
        viewPager.addOnPageChangeListener(this);
        navView.setOnNavigationItemSelectedListener(mOnNavigationItemSelectedListener);
    }

    @Override
    public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {
    }

    @Override
    public void onPageSelected(int position) {
    }

    @Override
    public void onPageScrollStateChanged(int state) {
        //state的状态有三个，0表示什么都没做，1正在滑动，2滑动完毕
        if(state==2){
            switch (viewPager.getCurrentItem()){
                case PAGE_HOME:
                    navView.setSelectedItemId(R.id.navigation_home);
                    break;
                case PAGE_NOTIFICATION:
                    navView.setSelectedItemId(R.id.navigation_notifications);
                    break;
                case PAGE_SETTINGS:
                    navView.setSelectedItemId(R.id.navigation_settings);
                    break;
            }
        }
    }
}
