package com.chengtao.beautifulled.activity;

import android.content.Context;
import android.content.IntentFilter;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.support.v7.app.AlertDialog;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.SeekBar;

import com.chengtao.beautifulled.R;
import com.chengtao.beautifulled.command.BeautifulLed;
import com.chengtao.beautifulled.command.LEDPositionCommand;
import com.chengtao.beautifulled.receiver.WifiStateReceiver;
import com.chengtao.pianoview.entity.Piano;
import com.chengtao.pianoview.impl.OnLoadAudioListener;
import com.chengtao.pianoview.impl.OnPianoClickListener;
import com.chengtao.pianoview.view.PianoView;
import com.dinuscxj.progressbar.CircleProgressBar;

public class MainActivity extends BaseActivity implements WifiStateReceiver.OnWifiStateListener,OnPianoClickListener,OnLoadAudioListener,SeekBar.OnSeekBarChangeListener{
    //---------------控件
    private SeekBar seekBar;
    private PianoView pianoView;
    private CircleProgressBar progressBar;
    //--------------指令
    private LEDPositionCommand ledPositionCommand;
    //对话框
    private AlertDialog dialog;
    //WIFI状态广播
    private WifiStateReceiver receiver;
    @Override
    protected int getLayoutId() {
        return R.layout.activity_main;
    }

    @Override
    protected void initView() {
        seekBar = getView(R.id.sb);
        pianoView = getView(R.id.pv);
        pianoView.setCanPress(false);
        //初始化对话框
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        View view = LayoutInflater.from(this).inflate(R.layout.dialog_loading_audio,null);
        progressBar = (CircleProgressBar) view.findViewById(R.id.line_progress);
        builder.setView(view);
        builder.setCancelable(false);
        dialog = builder.create();
    }

    @Override
    protected void initData() {
        //-------------初始化Socket
        initSockect(BeautifulLed.WIFI_HOST_IP,8080);
        //-------------初始化指令
        ledPositionCommand = new LEDPositionCommand();
        //初始化WIFI状态广播
        initReceiver();
    }

    private void initReceiver() {
        receiver = new WifiStateReceiver();
        receiver.setOnWifiStateListener(this);
        IntentFilter filter = new IntentFilter();
        filter.addAction("android.net.wifi.WIFI_STATE_CHANGED");
        filter.addAction("android.net.wifi.STATE_CHANGE");
        registerReceiver(receiver,filter);
    }

    @Override
    protected void setListener() {
        pianoView.setOnPianoClickListener(this);
        pianoView.setOnLoadMusicListener(this);
        seekBar.setOnSeekBarChangeListener(this);
    }

    @Override
    protected boolean isOrientationLandscape() {
        return true;
    }

    @Override
    public void onInitSuccess() {
        super.onInitSuccess();
    }

    @Override
    public void onInitFail(Exception e) {
        super.onInitFail(e);
    }

    @Override
    public void onFail(int id, Exception e) {
        super.onFail(id, e);
    }

    @Override
    public void onSuccess(int id) {
        super.onSuccess(id);
    }

    @Override
    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
        pianoView.scroll(i);
    }

    @Override
    public void onStartTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void onStopTrackingTouch(SeekBar seekBar) {

    }

    @Override
    public void loadPianoAudioStart() {
        dialog.show();
    }

    @Override
    public void loadPianoAudioFinish() {
        dialog.dismiss();
        showToast("加载音频成功,开始你的炫酷之旅吧~~");
    }

    @Override
    public void loadPianoAudioError(Exception e) {
        dialog.dismiss();
        showToast("加载音频失败了诶，试试重新加载一下吧");
    }

    @Override
    public void loadPianoAudioProgress(int progress) {
        progressBar.setProgress(progress);
    }

    @Override
    public void onPianoClick(Piano.PianoKeyType type, Piano.PianoVoice voice, int group, int positionOfGroup) {
            switch (voice){
                case DO:
                    ledPositionCommand.setCommandContent("0");
                    sendCommand(0,ledPositionCommand);
                    break;
                case RE:
                    ledPositionCommand.setCommandContent("1");
                    sendCommand(0,ledPositionCommand);
                    break;
                case MI:
                    ledPositionCommand.setCommandContent("2");
                    sendCommand(0,ledPositionCommand);
                    break;
                case FA:
                    ledPositionCommand.setCommandContent("3");
                    sendCommand(0,ledPositionCommand);
                    break;
                case SO:
                    ledPositionCommand.setCommandContent("4");
                    sendCommand(0,ledPositionCommand);
                    break;
                case LA:
                    ledPositionCommand.setCommandContent("5");
                    sendCommand(0,ledPositionCommand);
                    break;
                case SI:
                    ledPositionCommand.setCommandContent("6");
                    sendCommand(0,ledPositionCommand);
                    break;
            }
    }

    @Override
    public void onGood() {
        pianoView.setCanPress(true);
    }

    @Override
    public void onError(String message) {
        pianoView.setCanPress(false);
        showToast(message);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(receiver);
    }

}
