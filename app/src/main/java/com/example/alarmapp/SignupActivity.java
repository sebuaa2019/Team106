package com.example.alarmapp;

import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request.Method;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.example.alarmapp.Utils.AppController;
import com.example.alarmapp.Utils.Tools;


import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

import static com.example.alarmapp.Utils.URLConf.*;

public class SignupActivity extends AppCompatActivity {
    private EditText et_name;
    private EditText et_pwd;
    private EditText et_cpwd;
    private EditText et_phone;
    private Button btn_signup;
    private String username = "";
    private String userpwd = "";
    private String confirmpwd = "";
    private String userphone = "";

    // 返回到登录界面，可以直接返回即可
    @Override
    protected void onCreate(Bundle savedInstanceState){
        super.onCreate(savedInstanceState);
        ActionBar actionBar = getSupportActionBar();
        actionBar.hide();
        setContentView(R.layout.signup_layout);

        et_name = findViewById(R.id.set_userName);
        et_pwd = findViewById(R.id.set_password);
        et_cpwd = findViewById(R.id.et_confimrpassword);
        et_phone = findViewById(R.id.et_userphone);
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
        et_phone.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void afterTextChanged(Editable editable) {
                userphone = editable.toString();
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
        //注册
        //密码设置为6为长度以上，用户名不多于20个字符
        if(username.length() != 0 && userpwd.length() != 0 && confirmpwd.length() != 0
                && userphone.length() == 11){
            if(userpwd.equals(confirmpwd)){
                if(userpwd.length() >= 6){
                    //signup

                    String url = USING_URL + REGISTER;
                    final String tag = "json_signup";
                    Map<String, String> map = new HashMap<String, String>();
                    map.put("username", username);
                    map.put("password", Tools.md5(userpwd));
                    map.put("phone", userphone);
                    JSONObject params = new JSONObject(map);
                    JsonObjectRequest jsonObjectRequest = new JsonObjectRequest( Method.POST ,url, params,
                            new Response.Listener<JSONObject>() {
                                @Override
                                public void onResponse(JSONObject response) {
                                    Log.d(tag, response.toString());

                                    Intent intent = new Intent();
                                    intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK|Intent.FLAG_ACTIVITY_NEW_TASK);
                                    intent.setClass(SignupActivity.this, LoginActivity.class);
                                    startActivity(intent);
                                }
                            },
                            new Response.ErrorListener() {
                                @Override
                                public void onErrorResponse(VolleyError error) {
                                    Log.e(tag, error.toString());
                                }
                            }){
                        @Override
                        public Map<String, String> getHeaders() throws AuthFailureError {
                            HashMap<String, String> headers = new HashMap<String, String>();
                            headers.put("Content-Type", "application/json");
                            SharedPreferences sp = getSharedPreferences("conf", 0);
                            String token = sp.getString("token", "");
                            headers.put("token", token);
                            return headers;
                        }
                    };

                    AppController.getInstance().addToRequestQueue(jsonObjectRequest, tag);


                }
                else{
                    String info = "密码长度不应该小于6位";
                    Toast.makeText(getApplicationContext(), info, Toast.LENGTH_LONG).show();
                }
            }
            else{
                String info = "两次输入密码不一致";
                Toast.makeText(getApplicationContext(), info, Toast.LENGTH_LONG).show();
            }
        }
        else{
            String info = "请完整填写信息";
            Toast.makeText(getApplicationContext(), info, Toast.LENGTH_LONG).show();
        }
    }
}
