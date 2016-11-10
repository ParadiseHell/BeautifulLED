package com.chengtao.beautifulled.socket;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.support.annotation.Nullable;

/**
 * Created by ChengTao on 2016-11-09.
 */

public class UDPListenService extends Service {
    private UDPReceiveBinder binder;

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        binder = new UDPReceiveBinder();
        return super.onStartCommand(intent, flags, startId);
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return binder;
    }

    class UDPReceiveBinder  extends Binder{
        private UDPHandler udpHandler;

        public void setUdpHandler(UDPHandler udpHandler) {
            this.udpHandler = udpHandler;
        }

        public void startToListen(){

        }
    }
}
