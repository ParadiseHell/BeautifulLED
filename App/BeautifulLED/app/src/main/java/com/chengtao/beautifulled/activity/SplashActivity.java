package com.chengtao.beautifulled.activity;

import android.view.animation.AlphaAnimation;
import android.widget.LinearLayout;
import com.chengtao.beautifulled.R;

/**
 * Created by ChengTao on 2016-12-14.
 */

/**
 * 闪屏界面
 */
public class SplashActivity extends BaseActivity {
  private final static int DELAY_TIME = 2000;

  @Override protected int getLayoutId() {
    return R.layout.activity_splash;
  }

  @Override protected void initView() {
    LinearLayout linearLayout = getView(R.id.ll_splash);
    AlphaAnimation animation = new AlphaAnimation(0f, 1f);
    animation.setDuration(DELAY_TIME);
    linearLayout.setAnimation(animation);
  }

  @Override protected void initData() {
    mHandler.postDelayed(new Runnable() {
      @Override public void run() {
        PianoControlActivity.invoke(SplashActivity.this);
        SplashActivity.this.overridePendingTransition(android.R.anim.fade_out,
            android.R.anim.fade_in);
        SplashActivity.this.finish();
      }
    }, DELAY_TIME);
  }

  @Override protected void setListener() {

  }

  @Override protected boolean isFullScreen() {
    return true;
  }
}
