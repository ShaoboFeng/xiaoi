package com.zachary.shaobo.xiaoai.utils;

import android.util.Log;

import com.zachary.shaobo.xiaoai.model.Sensor;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;
/**
 * Created by shaobo on 2016/12/21.
 */

public class NetWorkConnection {
    private  final String TAG = NetWorkConnection.class.getSimpleName();
    private static NetWorkConnection sConnect = null;
    private String mHostName = "39.174.61.178";
    private int mPort = 8088;
    private Socket mSocket = null;
    public void setPort(int port){
        this.mPort = port;
    }
    public void setHostName(String hostName){
        this.mHostName = hostName;
    }
    public static NetWorkConnection getInstance(){
        synchronized (NetWorkConnection.class) {
            if (sConnect == null) {
                sConnect = new NetWorkConnection();
            }
            return sConnect;
        }
    }

    private NetWorkConnection() {
        try{
            mSocket = new Socket(mHostName, mPort);
        }catch (Exception e){
            Log.e(TAG,"Socket failed");
            mSocket = null;
        }
    }

    public void disconnect(){
        if(mSocket == null){
            return ;
        }
        try{
            mSocket.close();
        }catch (IOException e){
            Log.e(TAG,"socket error");
        }
    }

    public String getSocketData(Sensor sensor) {
        if(mSocket == null){
            return null;
        }
        byte buffer[] = new byte[1024];
        try{
            String socketData = sensor.getSensorType();
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(
                          mSocket.getOutputStream()));
            writer.write(socketData);
            writer.flush();
            InputStream inputStream = mSocket.getInputStream();

            int temp = 0;
            while ((temp = inputStream.read(buffer)) != -1) {
                if (temp > 512){
                    break;
                }
                Log.e(TAG,"buf:"+buffer.toString());
                break;
            }
        } catch (UnknownHostException e) {
                e.printStackTrace();
        } catch (IOException e) {
                e.printStackTrace();
        }
        return buffer.toString();
    }
}
