package com.chengtao.beautifulled.adapter;

import android.annotation.SuppressLint;
import android.content.Context;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import com.chengtao.beautifulled.R;
import com.chengtao.beautifulled.entity.Music;
import java.util.List;

/**
 * Author : ChengTao(chengtaolearn@163.com)
 * Date : 11/12/17
 * Time : 9:38 PM
 * Description :
 */

public class MusicAdapter extends BaseAdapter {
  private Context context;
  private List<Music> list;

  public MusicAdapter(Context context, List<Music> list) {
    this.context = context;
    this.list = list;
  }

  @Override public int getCount() {
    return list == null ? 0 : list.size();
  }

  @Override public Object getItem(int position) {
    return list.get(position);
  }

  @Override public long getItemId(int position) {
    return position;
  }

  @SuppressLint("InflateParams") @Override
  public View getView(int position, View convertView, ViewGroup parent) {
    ViewHolder holder;
    if (convertView == null) {
      convertView = LayoutInflater.from(context).inflate(R.layout.adapter_music, null, false);
      holder = new ViewHolder();
      holder.tvName = convertView.findViewById(R.id.tv_name);
      convertView.setTag(holder);
    } else {
      holder = (ViewHolder) convertView.getTag();
    }
    Music music = list.get(position);
    if (music != null) {
      holder.tvName.setText(TextUtils.isEmpty(music.getName()) ? "" : music.getName());
    }
    return convertView;
  }

  private class ViewHolder {
    private TextView tvName;
  }
}
