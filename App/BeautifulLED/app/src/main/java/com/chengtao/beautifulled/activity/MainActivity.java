package com.chengtao.beautifulled.activity;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;

import com.chengtao.beautifulled.R;
import com.chengtao.beautifulled.command.LEDPositionCommand;
import com.chengtao.beautifulled.socket.UDPSockect;
import com.chengtao.pianoview.entity.Piano;
import com.chengtao.pianoview.impl.OnLoadAudioListener;
import com.chengtao.pianoview.impl.OnPianoClickListener;
import com.chengtao.pianoview.view.PianoView;

public class MainActivity extends BaseActivity implements View.OnClickListener,OnPianoClickListener,OnLoadAudioListener,SeekBar.OnSeekBarChangeListener{
    //---------------控件
    private EditText etIp;
    private Button btnInit;
    private SeekBar seekBar;
    private PianoView pianoView;
    //--------------指令
    private LEDPositionCommand ledPositionCommand;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        if(getRequestedOrientation()!= ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE){
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        }
        super.onCreate(savedInstanceState);
        mContext = this;
        mHandler = new Handler();
        sockect = new UDPSockect(this);
        if (getLayoutId() != 0){
            setContentView(getLayoutId());
        }
        initView();
        setListener();
        initData();
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()){
            case R.id.btn_init:
                String ip = etIp.getText().toString();
                initSockect(ip,8080);
                break;
        }
    }

    @Override
    protected int getLayoutId() {
        return R.layout.activity_main;
    }

    @Override
    protected void initView() {
        etIp = getView(R.id.et_service_ip);
        btnInit = getView(R.id.btn_init);
        seekBar = getView(R.id.sb);
        pianoView = getView(R.id.pv);
    }

    @Override
    protected void initData() {
        ledPositionCommand = new LEDPositionCommand();
    }

    @Override
    protected void setListener() {
        btnInit.setOnClickListener(this);
        pianoView.setOnPianoClickListener(this);
        pianoView.setOnLoadMusicListener(this);
        seekBar.setOnSeekBarChangeListener(this);
    }

    @Override
    public void onInitSuccess() {
        super.onInitSuccess();
        showToast("INIT SUCCESS");
    }

    @Override
    public void onInitFail(Exception e) {
        super.onInitFail(e);
        showToast("INIT FAIL");
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
        showToast("loadPianoAudioStart");
    }

    @Override
    public void loadPianoAudioFinish() {
        showToast("loadPianoAudioFinish");
    }

    @Override
    public void loadPianoAudioError(Exception e) {
        showToast("loadPianoAudioError:"+e.getMessage());
    }

    @Override
    public void loadPianoAudioProgress(int progress) {
        showToast("loadPianoAudioProgress:"+progress);
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
}
