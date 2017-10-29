package com.chengtao.beautifulled.socket;

import android.util.Log;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;

/**
 * Created by ChengTao on 2016-11-07.
 */

public class UDPSocket {
    private DatagramSocket socket;
    private DatagramPacket packet;
    private InetAddress address;
    private int port = -1;
    private UDPHandler mHandler;
    private boolean isInit = false;

    public boolean isInit() {
        return isInit;
    }

    public UDPSocket(UDPListener listener){
        mHandler = new UDPHandler(listener);
    }

    private void setNull(){
        socket = null;
        address = null;
        port = -1;
        isInit = false;
    }

    public void initSocket(String ip,int port){
        try {
            setNull();
            socket = new DatagramSocket();
            address = InetAddress.getByName(ip);
            Log.e("TAG",ip+"------"+port);
            this.port  = port;
            if (socket != null && address != null  && port > 0){
                isInit = true;
                mHandler.sendInitSuccessMessage();
            }else {
                Exception e = new Exception("Init Fail");
                mHandler.sendInitFailMessage(e);
            }
        } catch (Exception e) {
            mHandler.sendInitFailMessage(e);
        }
    }

    public void sendCommand(final int id, final String cmd){
            new Thread() {
                @Override
                public void run() {
                    packet = new DatagramPacket(cmd.getBytes(), cmd.length(), address, port);
                    Log.e("TAG",cmd);
                    try {
                        socket.send(packet);
                        mHandler.sendSuccessMessage(id);
                    } catch (IOException e) {
                        mHandler.sendFailMessage(id, e);
                    }
                }
            }.start();
    }
}
