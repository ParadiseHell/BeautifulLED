package com.chengtao.beautifulled.activity;

import android.content.Context;
import android.content.Intent;
import com.chengtao.beautifulled.R;
import us.feras.mdv.MarkdownView;

/**
 * Author : ChengTao(chengtaolearn@163.com)
 * Date : 12/25/17
 * Time : 12:12 PM
 * Description :
 */

@SuppressWarnings("FieldCanBeLocal") public class ConfigActivity extends BaseActivity {
  private static final String CONFIG_FILE_PATH = "file:///android_asset/config.md";
  private MarkdownView mvConfig;

  @Override protected int getLayoutId() {
    return R.layout.activity_config;
  }

  @Override protected void initView() {
    setTitle(R.string.config);
    mvConfig = getView(R.id.mv_config);
    mvConfig.loadMarkdownFile(CONFIG_FILE_PATH);
  }

  @Override protected void initData() {

  }

  @Override protected void setListener() {

  }

  public static void invoke(Context context) {
    context.startActivity(new Intent(context, ConfigActivity.class));
  }
}
