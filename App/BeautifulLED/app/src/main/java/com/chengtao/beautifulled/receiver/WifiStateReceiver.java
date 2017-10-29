package com.chengtao.beautifulled.receiver;

/**
 * Created by ChengTao on 2016-12-14.
 */

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;

/**
 * 判断WIFI状态的广播
 */
@SuppressWarnings("SpellCheckingInspection")
public class WifiStateReceiver extends BroadcastReceiver{
    //WIFI名字
    public static final String WIFI_NAME = "PianoAurora";
    private OnWifiStateListener listener;
    @SuppressWarnings("ConstantConditions")
    @Override
    public void onReceive(Context context, Intent intent) {
        if (WifiManager.WIFI_STATE_CHANGED_ACTION.equals(intent.getAction())){
            int wifiState = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE,0);
            switch (wifiState){
                case WifiManager.WIFI_STATE_DISABLED:
                case WifiManager.WIFI_STATE_DISABLING:
                    if (listener != null){
                        listener.onError("请打开WIFI");
                    }
                    break;
            }
        }else if (WifiManager.NETWORK_STATE_CHANGED_ACTION.equals(intent.getAction())){
            WifiManager wifiManager = (WifiManager) context.getApplicationContext().getSystemService (Context.WIFI_SERVICE);
            if (wifiManager != null){
                WifiInfo wifiInfo = wifiManager.getConnectionInfo ();
                if (wifiInfo != null){
                    String ssid = wifiInfo.getSSID();
                    ssid = ssid.replace("\"","");
                    if (ssid.equals(WIFI_NAME)){
                        if (listener != null){
                            listener.onGood();
                        }
                    }else {
                        if (!ssid.equals("<unknown ssid>")){
                            if (listener != null){
                                listener.onError("请连接WIFI:"+WIFI_NAME);
                            }
                        }
                    }
                }
            }
        }
    }

    public interface OnWifiStateListener{
        void onGood();
        void onError(String message);
    }

     public void setOnWifiStateListener(OnWifiStateListener listener){
         this.listener = listener;
     }
}
