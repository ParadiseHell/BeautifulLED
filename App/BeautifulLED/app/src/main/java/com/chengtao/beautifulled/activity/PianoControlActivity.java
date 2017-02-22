package com.chengtao.beautifulled.activity;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AlertDialog;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;

import com.chengtao.beautifulled.R;
import com.chengtao.beautifulled.command.BeautifulLed;
import com.chengtao.beautifulled.command.LEDPositionCommand;
import com.chengtao.beautifulled.receiver.WifiStateReceiver;
import com.chengtao.beautifulled.utils.MusicUtils;
import com.chengtao.beautifulled.utils.PxDpUtils;
import com.chengtao.beautifulled.utils.SpUtils;
import com.chengtao.pianoview.entity.AutoPlayEntity;
import com.chengtao.pianoview.entity.Piano;
import com.chengtao.pianoview.impl.OnLoadAudioListener;
import com.chengtao.pianoview.impl.OnPianoAutoPlayListener;
import com.chengtao.pianoview.impl.OnPianoClickListener;
import com.chengtao.pianoview.impl.OnPianoDrawFinishListener;
import com.chengtao.pianoview.view.PianoView;
import com.dinuscxj.progressbar.CircleProgressBar;

import java.util.ArrayList;

/**
 * 钢琴控制界面
 */
public class PianoControlActivity extends BaseActivity implements View.OnClickListener,WifiStateReceiver.OnWifiStateListener,OnPianoClickListener,OnLoadAudioListener,SeekBar.OnSeekBarChangeListener,OnPianoDrawFinishListener,OnPianoAutoPlayListener{
    //--------------常量
    //钢琴视图图片总的宽度
    private static final int IMAGE_PIANO_BAR_TOTAL_WIDTH = 592;
    //钢琴视图图片左边偏移量（实际的钢琴开始位置）
    private static final int IMAGE_PIANO_BAR_OFFSET_WIDTH = 33;
    //seekbar拇指图片默认和背景偏移的dp值
    private static final float SEEKBAR_THUMB_DEFULT_OFFSET_DP = 14f;
    //---------------控件
    private SeekBar seekBar;
    private PianoView pianoView;
    private CircleProgressBar progressBar;
    private Button leftArrow;
    private Button rightArrow;
    private Button btnMusic;
    private ImageView ivTipAutoPlay;
    //--------------指令
    private LEDPositionCommand ledPositionCommand;
    //对话框
    private AlertDialog dialog;
    //WIFI状态广播
    private WifiStateReceiver receiver;
    //左右两个按键增加和减少的进度值
    private int scrollProgress = 0;
    //小星星音乐列表
    private ArrayList<AutoPlayEntity> litterStarList;
    //是否正在自动播放播放
    private boolean isAutoPlaying = false;
    @Override
    protected int getLayoutId() {
        return R.layout.activity_piano_control;
    }

    @Override
    protected void initView() {
        Log.e("TAG","initView");
        seekBar = getView(R.id.sb);
        pianoView = getView(R.id.pv);
        leftArrow = getView(R.id.iv_left_arrow);
        rightArrow = getView(R.id.iv_right_arrow);
        btnMusic = getView(R.id.iv_music);
        ivTipAutoPlay = getView(R.id.iv_tip_auto_play);
        ivTipAutoPlay.setVisibility(View.GONE);
        //初始化对话框
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        @SuppressLint("InflateParams") View view = LayoutInflater.from(this).inflate(R.layout.dialog_loading_audio,null);
        progressBar = (CircleProgressBar) view.findViewById(R.id.line_progress);
        builder.setView(view);
        builder.setCancelable(false);
        dialog = builder.create();
    }

    @Override
    protected void initData() {
        //-------------初始化Socket
        initSockect(BeautifulLed.WIFI_HOST_IP,BeautifulLed.WIFI_HOST_PORT);
        //-------------初始化指令
        ledPositionCommand = new LEDPositionCommand();
        //初始化WIFI状态广播
        initReceiver();
        litterStarList = MusicUtils.getLitterStarMusic();
        //
        seekBar.setProgress(50);
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
        pianoView.setFinishListener(this);
        pianoView.setAutoPlayListener(this);
        seekBar.setOnSeekBarChangeListener(this);
        leftArrow.setOnClickListener(this);
        rightArrow.setOnClickListener(this);
        btnMusic.setOnClickListener(this);
        ivTipAutoPlay.setOnClickListener(this);
    }

    @Override
    protected boolean isOrientationLandscape() {
        return true;
    }

    @Override
    protected boolean isFullScreen() {
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
        if (SpUtils.isFirstIn(mContext)){
            ivTipAutoPlay.setVisibility(View.VISIBLE);
        }else {
            ivTipAutoPlay.setOnClickListener(null);
        }
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
        showToast(message);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unregisterReceiver(receiver);
    }

    /**
     * 本界面跳转
     * @param activity Activity
     */
    public static void invoke(Activity activity){
        Intent intent = new Intent(activity,PianoControlActivity.class);
        activity.startActivity(intent);
    }

    @Override
    public void onClick(View view) {
        if (scrollProgress == 0) {
            try{
                scrollProgress = (pianoView.getLayoutWidth() * 100) / pianoView.getPianoWidth();
            }catch (Exception e){
                showToast(e.getMessage());
            }
        }
        int progress;
        switch (view.getId()){
            case R.id.iv_left_arrow://左移
                if (scrollProgress == 0){
                    progress = 0;
                }else {
                    progress = seekBar.getProgress() - scrollProgress;
                    if (progress < 0){
                        progress = 0;
                    }
                }
                seekBar.setProgress(progress);
                break;
            case R.id.iv_right_arrow://右移
                if (scrollProgress == 0){
                    progress = 100;
                }else {
                    progress = seekBar.getProgress() + scrollProgress;
                    if (progress > 100){
                        progress = 100;
                    }
                }
                seekBar.setProgress(progress);
                break;
            case R.id.iv_music://自动播放
                if (!isAutoPlaying){
                    isAutoPlaying = true;
                    autoPlayLitterStarMusic();
                }else {
                    showToast("正在自动播放小星星,请等待播放完成~~");
                }
                break;
            case R.id.iv_tip_auto_play:
                ivTipAutoPlay.setVisibility(View.GONE);
                SpUtils.setNotFirstIn(mContext);
                break;
        }
    }

    /**
     * 自动播放小星星音乐
     */
    private void autoPlayLitterStarMusic() {
        if (litterStarList != null && litterStarList.size() > 0){
            seekBar.setProgress(50);
            pianoView.autoPlay(litterStarList);
        }
    }

    @Override
    public void onPianoDrawFinish() {
        Log.e("TAG","pianoDrawFinish");
        //获取钢琴视图在手机上的宽度
        int pianobarTotalWidth = seekBar.getWidth() - seekBar.getPaddingLeft() - seekBar.getPaddingRight();
        Log.e("TAG","pianobarTotalWidth------"+pianobarTotalWidth);
        //获取钢琴视图在手机上的缩放
        float pianobarScale = (float)pianobarTotalWidth / IMAGE_PIANO_BAR_TOTAL_WIDTH;
        Log.e("TAG","pianobarScale------"+pianobarScale);
        //获取手机上钢琴视图的实际宽度
        int pianobarWidth = (int)(pianobarScale * (IMAGE_PIANO_BAR_TOTAL_WIDTH - 2 * IMAGE_PIANO_BAR_OFFSET_WIDTH));
        //获取钢琴视图拇指宽度
        int pianoThumbWith = (int)(((float) pianobarWidth / (float) pianoView.getPianoWidth()) * (float) pianoView.getLayoutWidth());
        //设置seekbar拇指图片
        Drawable drawable = ContextCompat.getDrawable(mContext,R.drawable.seekbar_thumb);
        Bitmap bitmap = ((BitmapDrawable)drawable).getBitmap();
        Drawable d = new BitmapDrawable(getResources(), Bitmap.createScaledBitmap(bitmap, pianoThumbWith, seekBar.getHeight(), true));
        seekBar.setThumb(d);
        //获取钢琴视图拇指资源
        int seekbarThumbOffset = -1 * (int)(IMAGE_PIANO_BAR_OFFSET_WIDTH * pianobarScale)
                + (int)PxDpUtils.convertDpToPixel(SEEKBAR_THUMB_DEFULT_OFFSET_DP,mContext);
        Log.e("TAG","seekbarThumbOffset------"+seekbarThumbOffset);
        //设置seekbar拇指图片的偏移
        seekBar.setThumbOffset(seekbarThumbOffset);
    }

    @Override
    public void onPianoAutoPlayStart() {
        showToast("开始自动播放小星星~~");
    }

    @Override
    public void onPianoAutoPlayEnd() {
        isAutoPlaying = false;
    }
}
