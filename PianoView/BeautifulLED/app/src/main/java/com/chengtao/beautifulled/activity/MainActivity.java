package com.chengtao.beautifulled.activity;

import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.chengtao.beautifulled.R;
import com.chengtao.beautifulled.command.LEDPositionCommand;

public class MainActivity extends BaseActivity implements View.OnClickListener{
    //---------------控件
    private EditText etIp;
    private Button btnInit;
    private Button btnDo;
    private Button btnRe;
    private Button btnMi;
    private Button btnFa;
    private Button btnSo;
    private Button btnLa;
    private Button btnSi;
    //--------------指令
    private LEDPositionCommand ledPositionCommand;
    //

    @Override
    public void onClick(View view) {
        switch (view.getId()){
            case R.id.btn_init:
                String ip = etIp.getText().toString();
                initSockect(ip,8080);
                break;
            case R.id.btn_do:
                ledPositionCommand.setCommandContent("1");
                sendCommand(0,ledPositionCommand);
                break;
            case R.id.btn_re:
                ledPositionCommand.setCommandContent("2");
                sendCommand(0,ledPositionCommand);
                break;
            case R.id.btn_mi:
                ledPositionCommand.setCommandContent("3");
                sendCommand(0,ledPositionCommand);
                break;
            case R.id.btn_fa:
                ledPositionCommand.setCommandContent("4");
                sendCommand(0,ledPositionCommand);
                break;
            case R.id.btn_so:
                ledPositionCommand.setCommandContent("5");
                sendCommand(0,ledPositionCommand);
                break;
            case R.id.btn_la:
                ledPositionCommand.setCommandContent("6");
                sendCommand(0,ledPositionCommand);
                break;
            case R.id.btn_si:
                ledPositionCommand.setCommandContent("7");
                sendCommand(0,ledPositionCommand);
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
        btnDo = getView(R.id.btn_do);
        btnRe = getView(R.id.btn_re);
        btnMi = getView(R.id.btn_mi);
        btnFa = getView(R.id.btn_fa);
        btnSo = getView(R.id.btn_so);
        btnLa = getView(R.id.btn_la);
        btnSi = getView(R.id.btn_si);
    }

    @Override
    protected void initData() {
        ledPositionCommand = new LEDPositionCommand();
    }

    @Override
    protected void setListener() {
        btnDo.setOnClickListener(this);
        btnRe.setOnClickListener(this);
        btnMi.setOnClickListener(this);
        btnFa.setOnClickListener(this);
        btnSo.setOnClickListener(this);
        btnLa.setOnClickListener(this);
        btnSi.setOnClickListener(this);
        btnInit.setOnClickListener(this);

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
        showToast("SEND FAIL");
    }

    @Override
    public void onSuccess(int id) {
        super.onSuccess(id);
        showToast("SEND SUCCESS");
    }
}
