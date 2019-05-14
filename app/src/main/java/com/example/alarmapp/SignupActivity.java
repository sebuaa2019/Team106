package com.example.alarmapp;

import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class SignupActivity extends AppCompatActivity {
    private EditText et_name;
    private EditText et_pwd;
    private EditText et_cpwd;
    private Button btn_signup;
    private String username = null;
    private String userpwd = null;
    private String confirmpwd = null;

    // 返回到登录界面，可以直接返回即可
    //TODO：增加一个电话号码的选项
    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        ActionBar actionBar = getSupportActionBar();
        actionBar.hide();
        setContentView(R.layout.signup_layout);

        et_name = findViewById(R.id.set_userName);
        et_pwd = findViewById(R.id.set_password);
        et_cpwd = findViewById(R.id.et_confimrpassword);
        btn_signup = findViewById(R.id.btn_signup);
        initEvent();
    }

    private void initEvent(){
        et_name.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void afterTextChanged(Editable editable) {
                username = editable.toString();
            }
        });
        et_pwd.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void afterTextChanged(Editable editable) {
                userpwd = editable.toString();
            }
        });
        et_cpwd.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void afterTextChanged(Editable editable) {
                confirmpwd = editable.toString();
            }
        });
        btn_signup.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                signup();
            }
        });
    }

    private void signup(){
        //登录
        String info = "username"+ username + "passwd" +userpwd + "confirm"+confirmpwd;
        Toast.makeText(getApplicationContext(), info, Toast.LENGTH_LONG).show();

        Intent intent = new Intent();
        intent.setClass(SignupActivity.this, LoginActivity.class);
        startActivity(intent);
    }
}
