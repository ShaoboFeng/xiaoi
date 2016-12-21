package com.zachary.shaobo.xiaoai;


import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;

import com.zachary.shaobo.xiaoai.utils.NetWorkConnection;
import com.zachary.shaobo.xiaoai.model.TemperatureSensor;

import java.util.concurrent.atomic.AtomicBoolean;

public class DevicesManagerActivity extends AppCompatActivity {
    private static final int DOWNSUCCESS=1;
    private  final String TAG = MainActivity.class.getSimpleName();
    private AtomicBoolean netWorkStop = new AtomicBoolean(false);
    private NetWorkConnection connect = null;

    private Handler  networkHandler=new  Handler(){
        @Override
        public void handleMessage(Message msg){
            super.handleMessage(msg);
            switch(msg.what){
                case  DOWNSUCCESS :
                    //操作UI
                    break;
            }
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Thread netThread = new Thread(new Runnable() {
            @Override
            public void run() {
                TemperatureSensor senser = new TemperatureSensor();
                connect = NetWorkConnection.getInstance();
                connect.getSocketData(senser);
            }
        });
        netThread.start();
    }


}
