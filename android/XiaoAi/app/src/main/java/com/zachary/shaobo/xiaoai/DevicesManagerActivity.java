package com.zachary.shaobo.xiaoai;


import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.TextView;

import com.zachary.shaobo.xiaoai.utils.NetWorkConnection;

import java.util.concurrent.atomic.AtomicBoolean;

public class DevicesManagerActivity extends AppCompatActivity {
    private static final int GETDATA = 1;
    private  final String TAG = MainActivity.class.getSimpleName();
    private AtomicBoolean netWorkStop = new AtomicBoolean(false);
    private NetWorkConnection connect = null;

    private TextView text = null;
    private Handler  networkHandler=new  Handler(){
        @Override
        public void handleMessage(Message msg){
            super.handleMessage(msg);
            switch(msg.what){
                case  GETDATA :
                    Bundle bundle = msg.getData();
                    if(bundle == null){
                        break;
                    }
                    String status = bundle.getString("status",null);
                    String temp = bundle.getString("temperature",null);
                    text.setText("status:"+status+"\nTemperature:"+temp);
                    Log.e("zachary","status:"+status+"\nTemperature:"+temp);
                    break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.smart_home);
        text = (TextView) findViewById(R.id.textView2);
    }
    @Override
    protected void onStart(){
        super.onStart();
        Log.e("zachary","onResume");
        Thread netThread = new Thread(new Runnable() {
            @Override
            public void run() {
                //TemperatureSensor senser = new TemperatureSensor();
                connect = NetWorkConnection.getInstance();
                Bundle bundle =  connect.getSensorData();
                Message msg = new Message();
                msg.what = GETDATA;
                msg.setData(bundle);
                networkHandler.sendMessage(msg);
            }
        });
        netThread.start();
    }

    @Override
    protected void onDestroy(){
        //connect.disconnect();
        super.onDestroy();
    }

}
