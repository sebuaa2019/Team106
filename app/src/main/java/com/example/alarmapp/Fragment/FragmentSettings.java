package com.example.alarmapp.Fragment;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.Toast;

import com.android.volley.AuthFailureError;
import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.example.alarmapp.LoginActivity;
import com.example.alarmapp.R;
import com.example.alarmapp.Utils.AppController;

import org.json.JSONObject;

import java.util.HashMap;
import java.util.Map;

import static com.example.alarmapp.Utils.URLConf.*;

public class FragmentSettings extends Fragment {
    private Button time_picker_button = null;
    private Button logout_button = null;
    private Button btn_modimg = null;
    private Button btn_modinfo = null;
    private Spinner spinner = null;
    private EditText et_modname = null;
    private EditText et_modphone = null;
    private int spin_time = 0;
    private String modName;
    private String modPhone;

    public FragmentSettings(){

    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState){
        View view = inflater.inflate(R.layout.fg_settings, container, false);
        Log.i("Fragment3", "settings");
        time_picker_button = view.findViewById(R.id.select_time_button);
        logout_button = view.findViewById(R.id.logout_button);
        btn_modimg = view.findViewById(R.id.btn_modimg);
        btn_modinfo = view.findViewById(R.id.btn_modinfo);
        initEvent();
        return view;
    }

    private void initEvent(){
        time_picker_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                showDialog();
            }
        });
        logout_button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                logout();
            }
        });
        btn_modimg.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                mod_img();
            }
        });
        btn_modinfo.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View view) {
                mod_info();
            }
        });
    }

    private void showDialog(){
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflater = getActivity().getLayoutInflater();
        View view = inflater.inflate(R.layout.timepicker_layout, null);
        builder.setView(view);
        spinner = view.findViewById(R.id.time_spinner);
        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
                spin_time = i;
            }

            @Override
            public void onNothingSelected(AdapterView<?> adapterView) {
                spin_time = 0;
            }
        });

        builder.setPositiveButton(R.string.confirm, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                //设置时间
                try {
                    //
                    SharedPreferences sp = getContext().getSharedPreferences("conf", 0);
                    final String token = sp.getString("token","");

                    String url = USING_URL + SETTIME;
                    final String tag = "json_set_time";
                    Map<String, Integer> map = new HashMap<String, Integer>();
                    map.put("interval", spin_time);
                    JSONObject params = new JSONObject(map);
                    JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, params,
                            new Response.Listener<JSONObject>() {
                                @Override
                                public void onResponse(JSONObject response) {
                                    try{
                                        if(response.getInt("status") == 0){
                                            Toast.makeText(getContext(), response.getString("msg"),
                                                    Toast.LENGTH_LONG).show();
                                        }
                                    }catch (Exception e){
                                        e.printStackTrace();
                                    }
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
                            headers.put("token", token);
                            return headers;
                        }
                    };
                    AppController.getInstance().addToRequestQueue(jsonObjectRequest, tag);

                }catch (NullPointerException e){
                    e.printStackTrace();
                }catch (Exception e){
                    e.printStackTrace();
                }
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

    private void logout(){
        try {
            SharedPreferences sp = getContext().getSharedPreferences("conf", 0);
            SharedPreferences.Editor editor = sp.edit();
            editor.putString("token", "");
            editor.putInt("id", -1);
            editor.putString("phone", "");
            editor.apply();
        }catch (NullPointerException e) {
            e.printStackTrace();
        }catch (Exception e){
            e.printStackTrace();
        }
        Intent intent = new Intent();
        intent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK|Intent.FLAG_ACTIVITY_NEW_TASK);
        intent.setClass(getContext(), LoginActivity.class);
        startActivity(intent);
    }

    private void mod_img(){
        //TODO: 头像
    }

    private void mod_info(){
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        LayoutInflater inflater = getActivity().getLayoutInflater();
        View view = inflater.inflate(R.layout.infomodify_layout, null);
        et_modname = view.findViewById(R.id.et_name);
        et_modphone = view.findViewById(R.id.et_userphone);
        builder.setView(view);

        et_modname.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void afterTextChanged(Editable editable) {
                modName = editable.toString();
            }
        });

        et_modphone.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void onTextChanged(CharSequence charSequence, int i, int i1, int i2) {

            }

            @Override
            public void afterTextChanged(Editable editable) {
                modPhone = editable.toString();
            }
        });

        builder.setPositiveButton(R.string.confirm, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialogInterface, int i) {
                String mod_info = "name:"+ modName + "phone:"+modPhone;
                Toast.makeText(getContext(), mod_info, Toast.LENGTH_LONG).show();
                //调用接口
                try {
                    //
                    SharedPreferences sp = getContext().getSharedPreferences("conf", 0);
                    final String token = sp.getString("token","");

                    String url = USING_URL + MODINFO;
                    final String tag = "json_modify_info";
                    Map<String, String> map = new HashMap<String, String>();
                    map.put("new_phone", modPhone);
                    map.put("new_name", modName);
                    JSONObject params = new JSONObject(map);
                    JsonObjectRequest jsonObjectRequest = new JsonObjectRequest(Request.Method.POST, url, params,
                            new Response.Listener<JSONObject>() {
                                @Override
                                public void onResponse(JSONObject response) {
                                    try{
                                        if(response.getInt("status") == 0){
                                            Toast.makeText(getContext(), response.getString("msg"),
                                                    Toast.LENGTH_LONG).show();
                                        }
                                    }catch (Exception e){
                                        e.printStackTrace();
                                    }
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
                            headers.put("token", token);
                            return headers;
                        }
                    };
                    AppController.getInstance().addToRequestQueue(jsonObjectRequest, tag);

                }catch (NullPointerException e){
                    e.printStackTrace();
                }catch (Exception e){
                    e.printStackTrace();
                }

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
