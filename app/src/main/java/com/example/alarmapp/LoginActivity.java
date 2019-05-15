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

import com.example.alarmapp.Utils.Tools;

public class LoginActivity extends AppCompatActivity {
    private EditText et_name;
    private EditText et_pwd;
    private Button btn_signup;
    private Button btn_login;
    private String username = "";
    private String passwd= "";
    private boolean empty_state = false;


    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        ActionBar actionBar = getSupportActionBar();
        actionBar.hide();
        setContentView(R.layout.login_layout);

        et_name = findViewById(R.id.et_userName);
        et_pwd = findViewById(R.id.et_password);
        btn_signup = findViewById(R.id.btn_signup);
        btn_login = findViewById(R.id.btn_login);

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
                passwd = editable.toString();
            }
        });

        btn_signup.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                toSignUp();
            }
        });

        btn_login.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                login();
            }
        });
    }

    private void toSignUp(){
        //跳转到注册界面
        Toast.makeText(getApplicationContext(), "hello", Toast.LENGTH_LONG).show();
        Intent intent = new Intent();
        intent.setClass(LoginActivity.this, SignupActivity.class);
        startActivity(intent);

    }

    private void login(){
        //验证
        String info = "username"+ username + "passwd" + passwd;
        Toast.makeText(getApplicationContext(), info, Toast.LENGTH_LONG).show();
//        if(username.length() != 0 && passwd.length() != 0){
//            String passwd_md5 = Tools.md5(passwd);

            Intent intent = new Intent();
            intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK|Intent.FLAG_ACTIVITY_NEW_TASK);
            intent.setClass(LoginActivity.this, MainActivity.class);
            startActivity(intent);
//        }
//        else{
//            Toast.makeText(getApplicationContext(), "请完整填写信息", Toast.LENGTH_LONG).show();
//        }
    }

}
