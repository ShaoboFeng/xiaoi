package com.zachary.shaobo.xiaoai.storage;

/**
 * Created by shaobo on 2016/12/14.
 */

public class DeviceStorage extends Storage {
    private static DeviceStorage mDeviceStorage = null;
    public static DeviceStorage getInstance() {
        synchronized (DeviceStorage.class){
            if (mDeviceStorage == null){
                mDeviceStorage = new DeviceStorage();
            }
            return mDeviceStorage;
        }
    }

    private DeviceStorage(){

    }

    /*
    * save device name and address to db
    * */
    public boolean bondDevice(String addr) {
        return true;
    }

    public String getBondDevice() {
        return "";
    }
}
