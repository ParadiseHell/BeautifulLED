package com.chengtao.beautifulled.activity;

import android.app.Activity;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.os.PersistableBundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.chengtao.beautifulled.R;
import com.chengtao.beautifulled.command.ICommand;
import com.chengtao.beautifulled.socket.UDPListener;
import com.chengtao.beautifulled.socket.UDPSockect;

/**
 * Created by ChengTao on 2016-11-07.
 */

public abstract class BaseActivity extends Activity implements UDPListener{
    protected Handler mHandler;
    protected Context mContext;
    protected UDPSockect sockect;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mContext = this;
        mHandler = new Handler();
        sockect = new UDPSockect(this);
        if (getLayoutId() != 0){
            Log.e("TAG",getLayoutId()+"");
            setContentView(getLayoutId());
        }
        initView();
        setListener();
        initData();
    }

    protected void initSockect(String ip,int port){
        sockect.initSocket(ip,port);
    }

    protected void sendCommand(int id,ICommand cmd){
        if (sockect.isInit()){
            if (cmd.getCommand() == null){
                showToast("请检查您的指令~");
            }else {
                sockect.sendCommand(id,cmd.getCommand());
            }
        }else {
            showToast("还没有初始化连接哦~");
        }
    }

    abstract protected int getLayoutId();

    abstract protected void initView();

    abstract protected void initData();

    abstract protected void setListener();

    protected <T extends View> T getView(int id){
        return (T) findViewById(id);
    }

    protected void showToast(String s){
        Toast.makeText(mContext,s,Toast.LENGTH_SHORT).show();
    }

    @Override
    public void onInitFail(Exception e) {

    }

    @Override
    public void onInitSuccess() {

    }

    @Override
    public void onSuccess(int id) {

    }

    @Override
    public void onFail(int id, Exception e) {

    }

    @Override
    public void onRecieve(int id, String response) {

    }
}
