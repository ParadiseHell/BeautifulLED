package com.chengtao.beautifulled.activity;

import com.chengtao.beautifulled.R;

/**
 * Created by ChengTao on 2016-12-14.
 */

/**
 * 闪屏界面
 */
public class SplashActivity extends BaseActivity{
    private final static int DELAY_TIME = 2000;
    @Override
    protected int getLayoutId() {
        return R.layout.activity_splash;
    }

    @Override
    protected void initView() {

    }

    @Override
    protected void initData() {
        mHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                MainActivity.invoke(SplashActivity.this);
                SplashActivity.this.overridePendingTransition(R.anim.splash_anim_in,R.anim.splash_animout);
                SplashActivity.this.finish();
            }
        },DELAY_TIME);
    }

    @Override
    protected void setListener() {

    }

    @Override
    protected boolean isOrientationLandscape() {
        return false;
    }
}
