package com.zachary.shaobo.xiaoai;

import android.content.ComponentName;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.design.widget.FloatingActionButton;
import android.support.design.widget.Snackbar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.View;

import com.zachary.shaobo.xiaoai.services.XiaoAiService;
import com.zachary.shaobo.xiaoai.storage.DeviceStorage;

public class NewVersionActivity extends AppCompatActivity {

    XiaoAiService msgService = null;
    ServiceConnection conn = new ServiceConnection() {

        @Override
        public void onServiceDisconnected(ComponentName name) {

        }

        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            msgService = ((XiaoAiService.LocalBinder)service).getService();

        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_new_version);
        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        FloatingActionButton fab = (FloatingActionButton) findViewById(R.id.fab);
        fab.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                DeviceStorage deviceStorage = DeviceStorage.getInstance();
                String addr = deviceStorage.getBondDevice();
                if(addr == null){
                    Snackbar.make(view, "please bong device first", Snackbar.LENGTH_LONG)
                        .setAction("Action", null).show();
                }
                //Intent intent = new Intent(XiaoAiService.class);
                //bindService(intent,conn, Context.BIND_AUTO_CREATE);
                /*notify service download hardware from bluetooth*/
                /*LocalBroadcastManager localBroadcastManager = LocalBroadcastManager.getInstance(this);
                localBroadcastManager.registerReceiver(mBroadcastReceiver, intentFilter);


                localBroadcastManager.unregisterReceiver(mBroadcastReceiver);

                Intent intent = new Intent();
                intent.setAction(BROADCAST_ACTION);
                intent.putExtra("name", "qqyumidi");
//sendBroadcast(intent);
//发送应用内广播
                localBroadcastManager.sendBroadcast(intent);
                */
            }
        });
    }

}
