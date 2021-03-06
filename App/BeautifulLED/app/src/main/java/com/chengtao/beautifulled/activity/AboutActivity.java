package com.chengtao.beautifulled.activity;

import android.content.Context;
import android.content.Intent;
import android.support.design.widget.FloatingActionButton;
import android.text.Html;
import android.text.method.LinkMovementMethod;
import android.widget.TextView;
import com.chengtao.beautifulled.R;

/**
 * Author : ChengTao(chengtaolearn@163.com)
 * Date : 12/24/17
 * Time : 10:48 AM
 * Description :
 */

@SuppressWarnings("FieldCanBeLocal") public class AboutActivity extends BaseActivity {
  private TextView tvProjectAddress;
  private FloatingActionButton btnConfig;

  @Override protected int getLayoutId() {
    return R.layout.activity_about;
  }

  @Override protected void initView() {
    setTitle(R.string.about);
    tvProjectAddress = getView(R.id.tv_project_address);
    tvProjectAddress.setText(Html.fromHtml(getString(R.string.project_address)));
    tvProjectAddress.setMovementMethod(LinkMovementMethod.getInstance());
    btnConfig = getView(R.id.btn_config);
    btnConfig.setOnClickListener(v -> ConfigActivity.invoke(mContext));
  }

  @Override protected void initData() {

  }

  @Override protected void setListener() {

  }

  public static void invoke(Context context) {
    context.startActivity(new Intent(context, AboutActivity.class));
  }
}
