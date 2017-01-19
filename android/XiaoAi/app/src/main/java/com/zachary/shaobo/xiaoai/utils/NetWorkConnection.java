package com.zachary.shaobo.xiaoai.utils;

import android.os.Bundle;
import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Iterator;

/**
 * Created by shaobo on 2016/12/21.
 */

public class NetWorkConnection {
    private  final String TAG = NetWorkConnection.class.getSimpleName();
    private static NetWorkConnection sConnect = null;
    private String mHostName = "39.174.61.178";
    private int mPort = 8081;
    private static Socket mSocket = null;
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

    public String encapsulated(String data) {
        return "####"+data+"****";
    }

    private boolean isPackage(String msg) {
        return true;
    }

    public Bundle getSensorData() {
        if(mSocket == null){
            Log.e(TAG,"mSocket is null,getSensorData");
            return null;
        }
        if(mSocket.isClosed()){
            Log.e(TAG,"socket is closed");
            return null;
        }
        Log.d(TAG,"getSensorData");
        byte buffer[] = new byte[1024];
        byte revbuf[] = new byte[513];
        try{
            //String socketData = sensor.getSensorType();
            JSONObject json = new JSONObject();
            json.put("action","get");
            BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(
                          mSocket.getOutputStream()));
            writer.write(encapsulated(json.toString()));
            writer.flush();
            InputStream inputStream = mSocket.getInputStream();
            int temp = 0;
            while ((temp = inputStream.read(revbuf)) != -1) {
                if (temp > 512){
                    break;
                }
                System.arraycopy(revbuf,0,buffer,0,temp);
                String msg = new String(buffer, 0, temp, "utf8");
                if(isPackage(msg)) {
                    JSONObject ret = new JSONObject(msg.substring(4,msg.length()-4));
                    Bundle b = new Bundle();
                    Iterator iter = ret.keys();
                    while(iter.hasNext()){
                        String key = (String)iter.next();
                        String value = ret.getString(key);
                        b.putString(key,value);
                    }
                    return b;
                    //return msg;
                }
                break;
            }
        } catch (UnknownHostException e) {
                e.printStackTrace();
        } catch (IOException e) {
                e.printStackTrace();
        } catch (JSONException e) {
                e.printStackTrace();
        }
        return null;
    }
}
