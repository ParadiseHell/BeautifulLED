package com.chengtao.beautifulled.activity;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.ColorDrawable;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.PowerManager;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AlertDialog;
import android.text.TextUtils;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.ListView;
import android.widget.PopupWindow;
import android.widget.SeekBar;
import android.widget.TextView;
import com.chengtao.beautifulled.R;
import com.chengtao.beautifulled.adapter.MusicAdapter;
import com.chengtao.beautifulled.command.BeautifulLed;
import com.chengtao.beautifulled.command.LEDPositionCommand;
import com.chengtao.beautifulled.database.dao.MusicDao;
import com.chengtao.beautifulled.database.impl.MusicDaoImpl;
import com.chengtao.beautifulled.entity.Music;
import com.chengtao.beautifulled.receiver.WifiStateReceiver;
import com.chengtao.beautifulled.utils.PxDpUtils;
import com.chengtao.beautifulled.utils.SpUtils;
import com.chengtao.pianoview.entity.AutoPlayEntity;
import com.chengtao.pianoview.entity.Piano;
import com.chengtao.pianoview.listener.OnLoadAudioListener;
import com.chengtao.pianoview.listener.OnPianoAutoPlayListener;
import com.chengtao.pianoview.listener.OnPianoListener;
import com.chengtao.pianoview.utils.AutoPlayUtils;
import com.chengtao.pianoview.utils.PianoConvertUtils;
import com.chengtao.pianoview.view.PianoView;
import com.dinuscxj.progressbar.CircleProgressBar;
import java.io.File;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * 钢琴控制界面
 */
public class PianoControlActivity extends BaseActivity
    implements View.OnClickListener, WifiStateReceiver.OnWifiStateListener, OnPianoListener,
    OnPianoAutoPlayListener, SeekBar.OnSeekBarChangeListener, OnLoadAudioListener,
    ListView.OnItemClickListener, MusicAdapter.SwipeListener {
  //--------------常量
  private static final String TAG = "PianoControlActivity";
  private static final long MAX_FILE_SIZE = 20 * 1024;
  private static final int FILE_REQUEST_CODE = 1;
  private static final int WRITE_EXTERNAL_STORAGE_REQUEST_CODE = 2;
  private static final int SOUND_POOL_MAX_STREAM = 10;
  private static final long WAKELOCK_TIME_OUT = 30 * 60 * 1000L;
  //钢琴视图图片总的宽度
  private static final int IMAGE_PIANO_BAR_TOTAL_WIDTH = 592;
  //钢琴视图图片左边偏移量（实际的钢琴开始位置）
  private static final int IMAGE_PIANO_BAR_OFFSET_WIDTH = 33;
  //seek bar拇指图片默认和背景偏移的dp值
  private static final float SEEK_BAR_THUMB_DEFAULT_OFFSET_DP = 14f;
  //---------------控件
  private SeekBar seekBar;
  private PianoView pianoView;
  private CircleProgressBar progressBar;
  private Button leftArrow;
  private Button rightArrow;
  private Button btnMore;
  private PopupWindow morePopupWindow;
  private TextView tvMusicList;
  private TextView tvAddMusic;
  private TextView tvAbout;
  private PopupWindow musicListPopupWindow;
  private ListView lvMusic;
  private MusicAdapter adapter;
  //--------------指令
  private LEDPositionCommand ledPositionCommand;
  //对话框
  private AlertDialog dialog;
  //WIFI状态广播
  private WifiStateReceiver receiver;
  //左右两个按键增加和减少的进度值
  private int scrollProgress = 0;
  //----数据库
  private MusicDao musicDao;
  //----其他
  private String currentMusicName;
  private List<Music> musicList = new ArrayList<>();
  private Map<String, List<AutoPlayEntity>> musicMap = new HashMap<>();
  private PowerManager.WakeLock wakeLock;

  @Override protected int getLayoutId() {
    return R.layout.activity_piano_control;
  }

  @Override protected void initView() {
    seekBar = getView(R.id.sb);
    pianoView = getView(R.id.pv);
    pianoView.setSoundPollMaxStream(SOUND_POOL_MAX_STREAM);
    leftArrow = getView(R.id.iv_left_arrow);
    rightArrow = getView(R.id.iv_right_arrow);
    btnMore = getView(R.id.iv_more);
    //初始化对话框
    AlertDialog.Builder builder = new AlertDialog.Builder(this);
    @SuppressLint("InflateParams") View view =
        LayoutInflater.from(this).inflate(R.layout.dialog_loading_audio, null);
    progressBar = view.findViewById(R.id.line_progress);
    builder.setView(view);
    builder.setCancelable(false);
    dialog = builder.create();
    //
    @SuppressLint("InflateParams") View moreView =
        LayoutInflater.from(this).inflate(R.layout.popupwindow_more, null, false);
    tvMusicList = moreView.findViewById(R.id.tv_music_list);
    tvAddMusic = moreView.findViewById(R.id.tv_add_music);
    tvAbout = moreView.findViewById(R.id.tv_about);
    morePopupWindow = new PopupWindow(moreView, ViewGroup.LayoutParams.WRAP_CONTENT,
        ViewGroup.LayoutParams.WRAP_CONTENT, true);
    morePopupWindow.setBackgroundDrawable(new ColorDrawable());
    //
    @SuppressLint("InflateParams") View musicListView =
        LayoutInflater.from(this).inflate(R.layout.popupwindow_music_list, null, false);
    lvMusic = musicListView.findViewById(R.id.lv_music);
    musicListPopupWindow = new PopupWindow(musicListView, ViewGroup.LayoutParams.WRAP_CONTENT,
        ViewGroup.LayoutParams.MATCH_PARENT, true);
    //
    PowerManager powerManager = (PowerManager) getSystemService(POWER_SERVICE);
    if (powerManager != null) {
      wakeLock = powerManager.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, TAG);
    }
  }

  @Override protected void initData() {
    musicDao = new MusicDaoImpl();
    if (SpUtils.isFirstIn(mContext)) {//第一次进入加载默认乐谱
      SpUtils.setNotFirstIn(mContext);
      AssetManager assetManager = getAssets();
      if (assetManager != null) {
        try {
          for (String name : assetManager.list("music")) {
            Object[] result = PianoConvertUtils.convertByInputStream(
                assetManager.open("music" + File.separator + name));
            musicDao.insertMusic((String) result[0], (String) result[1]);
          }
        } catch (Throwable throwable) {
          handleConfigError(throwable);
        }
      }
    }
    //-------------初始化Socket
    initSocket(BeautifulLed.WIFI_HOST_IP, BeautifulLed.WIFI_HOST_PORT);
    //-------------初始化指令
    ledPositionCommand = new LEDPositionCommand();
    //初始化WIFI状态广播
    initReceiver();
    //
    adapter = new MusicAdapter(this, musicList, lvMusic);
    adapter.setSwipeListener(this);
    lvMusic.setAdapter(adapter);
  }

  /**
   * 处理配置文件错误
   *
   * @param throwable 异常
   */
  private void handleConfigError(Throwable throwable) {
    if (throwable != null && throwable.getMessage() != null) {
      switch (throwable.getMessage()) {
        case PianoConvertUtils.Error.FILE_NOT_EXIT:
          showToast(getString(R.string.file_not_exist));
          break;
        case PianoConvertUtils.Error.READ_FILE_EXCEPTION:
          showToast(getString(R.string.read_file_exception));
          break;
        case PianoConvertUtils.Error.CONFIG_FILE_WRONG:
          showToast(getString(R.string.config_file_wrong));
          break;
        case PianoConvertUtils.Error.TUNE_LENGTH_NOT_ONE:
          showToast(getString(R.string.tune_length_not_one));
          break;
        case PianoConvertUtils.Error.TUNE_NOT_IN_RANGE:
          showToast(getString(R.string.tune_not_in_range));
          break;
        case PianoConvertUtils.Error.FREQUENCY_NOT_NUMBER:
          showToast(getString(R.string.frequency_not_number));
          break;
        case PianoConvertUtils.Error.FREQUENCY_NOT_IN_RANGE:
          showToast(getString(R.string.frequency_not_in_range));
          break;
        case PianoConvertUtils.Error.NO_MUSIC_NAME:
          showToast(getString(R.string.no_music_name));
          break;
        default:
          if (throwable.getMessage().contains(PianoConvertUtils.Error.MUSIC_NOTE_CONFIG_WRONG)) {
            String message = throwable.getMessage();
            String note = message.substring(message.lastIndexOf(":") + 1);
            showToast(getString(R.string.music_note_config_wrong, note));
          } else {
            showToast(getString(R.string.unknown_error));
            Log.e("TAG", "handleConfigError(PianoControlActivity.java:" + Thread.currentThread()
                .getStackTrace()[2].getLineNumber() + ")" + "throwable:" + throwable.getMessage());
          }
          break;
      }
    } else {
      showToast(getString(R.string.unknown_error));
    }
  }

  private void initReceiver() {
    receiver = new WifiStateReceiver();
    receiver.setOnWifiStateListener(this);
    IntentFilter filter = new IntentFilter();
    filter.addAction("android.net.wifi.WIFI_STATE_CHANGED");
    filter.addAction("android.net.wifi.STATE_CHANGE");
    registerReceiver(receiver, filter);
  }

  @Override protected void setListener() {
    pianoView.setLoadAudioListener(this);
    pianoView.setAutoPlayListener(this);
    pianoView.setPianoListener(this);
    pianoView.setAutoPlayListener(this);
    seekBar.setOnSeekBarChangeListener(this);
    leftArrow.setOnClickListener(this);
    rightArrow.setOnClickListener(this);
    btnMore.setOnClickListener(this);
    morePopupWindow.setOnDismissListener(() -> {
      WindowManager.LayoutParams lp = ((Activity) mContext).getWindow().getAttributes();
      lp.alpha = 1f;
      ((Activity) mContext).getWindow().setAttributes(lp);
    });
    musicListPopupWindow.setOnDismissListener(() -> {
      WindowManager.LayoutParams lp = ((Activity) mContext).getWindow().getAttributes();
      lp.alpha = 1f;
      ((Activity) mContext).getWindow().setAttributes(lp);
    });
    tvMusicList.setOnClickListener(this);
    tvAddMusic.setOnClickListener(this);
    tvAbout.setOnClickListener(this);
    lvMusic.setOnItemClickListener(this);
  }

  @Override protected void onResume() {
    super.onResume();
    if (wakeLock != null) {
      wakeLock.acquire(WAKELOCK_TIME_OUT);
    }
  }

  @Override protected void onStop() {
    super.onStop();
    if (wakeLock != null) {
      wakeLock.release();
    }
  }

  @Override protected boolean isOrientationLandscape() {
    return true;
  }

  @Override protected boolean isFullScreen() {
    return true;
  }

  @Override public void onInitSuccess() {
    super.onInitSuccess();
  }

  @Override public void onInitFail(Exception e) {
    super.onInitFail(e);
  }

  @Override public void onFail(int id, Exception e) {
    super.onFail(id, e);
  }

  @Override public void onSuccess(int id) {
    super.onSuccess(id);
  }

  @Override public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
    pianoView.scroll(i);
  }

  @Override public void onStartTrackingTouch(SeekBar seekBar) {

  }

  @Override public void onStopTrackingTouch(SeekBar seekBar) {

  }

  @Override public void loadPianoAudioStart() {
    dialog.show();
  }

  @Override public void loadPianoAudioFinish() {
    dialog.dismiss();
    showToast(getString(R.string.load_success));
  }

  @Override public void loadPianoAudioError(Exception e) {
    dialog.dismiss();
    showToast(getString(R.string.load_fail));
  }

  @Override public void loadPianoAudioProgress(int progress) {
    progressBar.setProgress(progress);
  }

  @Override public void onPianoInitFinish() {
    //获取钢琴视图在手机上的宽度
    int pianoBarTotalWidth =
        seekBar.getWidth() - seekBar.getPaddingLeft() - seekBar.getPaddingRight();
    //获取钢琴视图在手机上的缩放
    float pianoBarScale = (float) pianoBarTotalWidth / IMAGE_PIANO_BAR_TOTAL_WIDTH;
    //获取手机上钢琴视图的实际宽度
    int pianoBarWidth =
        (int) (pianoBarScale * (IMAGE_PIANO_BAR_TOTAL_WIDTH - 2 * IMAGE_PIANO_BAR_OFFSET_WIDTH));
    //获取钢琴视图拇指宽度
    int pianoThumbWith = (int) (((float) pianoBarWidth / (float) pianoView.getPianoWidth())
        * (float) pianoView.getLayoutWidth());
    //设置seek bar拇指图片
    Drawable drawable = ContextCompat.getDrawable(mContext, R.drawable.seekbar_thumb);
    Bitmap bitmap = ((BitmapDrawable) drawable).getBitmap();
    Drawable d = new BitmapDrawable(getResources(),
        Bitmap.createScaledBitmap(bitmap, pianoThumbWith, seekBar.getHeight(), true));
    seekBar.setThumb(d);
    //获取钢琴视图拇指资源
    int seekBarThumbOffset = -1 * (int) (IMAGE_PIANO_BAR_OFFSET_WIDTH * pianoBarScale)
        + (int) PxDpUtils.convertDpToPixel(SEEK_BAR_THUMB_DEFAULT_OFFSET_DP, mContext);
    //设置seek bar拇指图片的偏移
    seekBar.setThumbOffset(seekBarThumbOffset);
    //
    seekBar.setProgress(50);
  }

  @Override public void onPianoClick(Piano.PianoKeyType type, Piano.PianoVoice voice, int group,
      int positionOfGroup) {
    switch (voice) {
      case DO:
        ledPositionCommand.setCommandContent("0");
        sendCommand(0, ledPositionCommand);
        break;
      case RE:
        ledPositionCommand.setCommandContent("1");
        sendCommand(0, ledPositionCommand);
        break;
      case MI:
        ledPositionCommand.setCommandContent("2");
        sendCommand(0, ledPositionCommand);
        break;
      case FA:
        ledPositionCommand.setCommandContent("3");
        sendCommand(0, ledPositionCommand);
        break;
      case SO:
        ledPositionCommand.setCommandContent("4");
        sendCommand(0, ledPositionCommand);
        break;
      case LA:
        ledPositionCommand.setCommandContent("5");
        sendCommand(0, ledPositionCommand);
        break;
      case SI:
        ledPositionCommand.setCommandContent("6");
        sendCommand(0, ledPositionCommand);
        break;
    }
  }

  @Override public void onGood() {
    pianoView.setCanPress(true);
  }

  @Override public void onError(String message) {
    showToast(message);
  }

  @Override protected void onDestroy() {
    super.onDestroy();
    unregisterReceiver(receiver);
  }

  /**
   * 本界面跳转
   *
   * @param activity Activity
   */
  public static void invoke(Activity activity) {
    Intent intent = new Intent(activity, PianoControlActivity.class);
    activity.startActivity(intent);
  }

  @Override public void onClick(View view) {
    if (scrollProgress == 0) {
      try {
        scrollProgress = (pianoView.getLayoutWidth() * 100) / pianoView.getPianoWidth();
      } catch (Exception e) {
        showToast(e.getMessage());
      }
    }
    int progress;
    switch (view.getId()) {
      case R.id.iv_left_arrow://左移
        if (scrollProgress == 0) {
          progress = 0;
        } else {
          progress = seekBar.getProgress() - scrollProgress;
          if (progress < 0) {
            progress = 0;
          }
        }
        seekBar.setProgress(progress);
        break;
      case R.id.iv_right_arrow://右移
        if (scrollProgress == 0) {
          progress = 100;
        } else {
          progress = seekBar.getProgress() + scrollProgress;
          if (progress > 100) {
            progress = 100;
          }
        }
        seekBar.setProgress(progress);
        break;
      case R.id.iv_more://更多
        if (morePopupWindow != null) {
          WindowManager.LayoutParams lp = this.getWindow().getAttributes();
          lp.alpha = 0.5f;//设置阴影透明度
          this.getWindow().setAttributes(lp);
          morePopupWindow.showAtLocation(getWindow().getDecorView(), Gravity.CENTER, 0, 0);
        }
        break;
      case R.id.tv_music_list:
        if (morePopupWindow != null) {
          morePopupWindow.dismiss();
        }
        addMusicList();
        adapter.notifyDataSetChanged();
        if (musicListPopupWindow != null) {
          WindowManager.LayoutParams lp = this.getWindow().getAttributes();
          lp.alpha = 0.5f;//设置阴影透明度
          this.getWindow().setAttributes(lp);
          musicListPopupWindow.showAtLocation(getWindow().getDecorView(), Gravity.CENTER, 0, 0);
        }
        break;
      case R.id.tv_add_music:
        if (ContextCompat.checkSelfPermission(mContext, Manifest.permission.WRITE_EXTERNAL_STORAGE)
            == PackageManager.PERMISSION_GRANTED) {
          chooseConfigFileFromStorage();
        } else {
          ActivityCompat.requestPermissions(PianoControlActivity.this,
              new String[] { Manifest.permission.WRITE_EXTERNAL_STORAGE },
              WRITE_EXTERNAL_STORAGE_REQUEST_CODE);
        }
        break;
      case R.id.tv_about:
        if (morePopupWindow != null) {
          morePopupWindow.dismiss();
          AboutActivity.invoke(mContext);
        }
        break;
      default:
        break;
    }
  }

  @Override public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
      @NonNull int[] grantResults) {
    switch (requestCode) {
      case WRITE_EXTERNAL_STORAGE_REQUEST_CODE:
        if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
          chooseConfigFileFromStorage();
        } else {
          showToast(getString(R.string.ask_permission));
        }
        break;
      default:
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        break;
    }
  }

  private void chooseConfigFileFromStorage() {
    Intent intent = new Intent();
    intent.setType("*/*");
    intent.setAction(Intent.ACTION_GET_CONTENT);
    startActivityForResult(intent, FILE_REQUEST_CODE);
  }

  private void addMusicList() {
    if (musicList.isEmpty()) {
      List<Music> list = musicDao.queryAllMusics();
      if (list != null) {
        musicList.addAll(list);
      }
    }
  }

  @Override public void onPianoAutoPlayStart() {
    if (!TextUtils.isEmpty(currentMusicName)) {
      showToast(getString(R.string.auto_play, currentMusicName));
    }
  }

  @Override public void onPianoAutoPlayEnd() {
    //播放完成后,钢琴控件可控
    showToast(getString(R.string.auto_paly_end));
    pianoView.setCanPress(true);
  }

  @Override protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    switch (requestCode) {
      case FILE_REQUEST_CODE:
        if (morePopupWindow != null) {
          morePopupWindow.dismiss();
        }
        if (resultCode == RESULT_OK) {
          if (data != null) {
            Uri uri = data.getData();
            if (uri != null) {
              try {
                InputStream is = getContentResolver().openInputStream(uri);
                if (is == null) {
                  showToast(getString(R.string.file_not_exist));
                  return;
                }
                if (is.available() > MAX_FILE_SIZE) {
                  showToast(getString(R.string.file_too_big));
                  return;
                }
                Object[] result = PianoConvertUtils.convertByInputStream(is);
                String name = (String) result[0];
                String configString = (String) result[1];
                Music music = new Music();
                music.setName(name);
                music.setConfigString(configString);
                //更新列表
                boolean updated = false;
                addMusicList();
                for (int i = 0; i < musicList.size(); i++) {
                  if (name.equals(musicList.get(i).getName())) {
                    musicList.set(i, music);
                    updated = true;
                    break;
                  }
                }
                if (!updated) {
                  musicList.add(music);
                }
                adapter.notifyDataSetChanged();
                //数据库
                musicDao.insertMusic(name, configString);
                showToast(getString(R.string.add_music_success));
              } catch (Throwable throwable) {
                handleConfigError(throwable);
              }
            }
          }
        }
        break;
      default:
        super.onActivityResult(requestCode, resultCode, data);
        break;
    }
  }

  @Override public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
    if (musicListPopupWindow != null) {
      musicListPopupWindow.dismiss();
    }
    Music music = musicList.get(position);
    if (music != null) {
      if (!musicMap.containsKey(music.getName())) {
        try {
          Object[] result = PianoConvertUtils.convertByConfigString(music.getConfigString());
          String name = (String) result[0];
          @SuppressWarnings("unchecked") ArrayList<AutoPlayEntity> keys =
              AutoPlayUtils.convertToAutoPlayEntityList(
                  (List<PianoConvertUtils.PianoKey>) result[2]);
          if (!TextUtils.isEmpty(name) && keys != null) {
            musicMap.put(name, keys);
          }
        } catch (Throwable throwable) {
          handleConfigError(throwable);
        }
      }
      currentMusicName = music.getName();
      pianoView.autoPlay(musicMap.get(music.getName()));
    }
  }

  @Override public void onDelete(int position) {
    if (position >= 0 && position < musicList.size()) {
      Music music = musicList.get(position);
      musicList.remove(position);
      adapter.notifyDataSetChanged();
      musicDao.deleteMusicByName(music.getName());
    }
  }
}
