package com.zachary.shaobo.xiaoai.services;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;

import com.zachary.shaobo.xiaoai.storage.Storage;

/**
 * Created by shaobo on 2016/12/14.
 */

public class XiaoAiService extends Service {
    private final static String TAG = XiaoAiService.class.getSimpleName();

    BluetoothService bluetoothService = null;

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        return super.onUnbind(intent);
    }

    private final IBinder mBinder = new LocalBinder();

    public class LocalBinder extends Binder {
        public XiaoAiService getService() {
            return XiaoAiService.this;
        }
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId){
        if(bluetoothService == null){
            return START_STICKY;
        }
        bluetoothService.initialize(this);
        try{
            String address = Storage.getDevicesAddress();
            bluetoothService.connect(this,address);
        }catch (Exception e){

        }
        return START_STICKY;
    }

    @Override
    public void onDestroy(){
        if(bluetoothService != null) {
            bluetoothService.close();
        }
        super.onDestroy();
    }
}
