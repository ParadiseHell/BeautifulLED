package com.chengtao.beautifulled.adapter;

import android.annotation.SuppressLint;
import android.content.Context;
import android.text.TextUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;
import com.chengtao.beautifulled.R;
import com.chengtao.beautifulled.entity.Music;
import com.daimajia.swipe.SwipeLayout;
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
  private SwipeListener listener;
  private ListView lvMusic;
  private int lastSwipePosition = -1;

  public MusicAdapter(Context context, List<Music> list, ListView lvMusic) {
    this.context = context;
    this.list = list;
    this.lvMusic = lvMusic;
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
      holder.btnDelete = convertView.findViewById(R.id.btn_delete);
      holder.slMusic = convertView.findViewById(R.id.sl_music);
      convertView.setTag(holder);
    } else {
      holder = (ViewHolder) convertView.getTag();
    }
    Music music = list.get(position);
    if (music != null) {
      holder.tvName.setText(TextUtils.isEmpty(music.getName()) ? "" : music.getName());
      holder.btnDelete.setOnClickListener(v -> {
        holder.slMusic.close();
        if (listener != null) {
          listener.onDelete(position);
        }
      });
      holder.slMusic.addSwipeListener(new SwipeLayout.SwipeListener() {
        @Override public void onStartOpen(SwipeLayout layout) {
          if (lastSwipePosition != position) {
            Log.e("TAG", "onStartOpen(MusicAdapter.java:" + Thread.currentThread()
                .getStackTrace()[2].getLineNumber() + ")" + "position:" + position);
            closeByPosition(lastSwipePosition);
          }
        }

        @Override public void onOpen(SwipeLayout layout) {
          Log.e("TAG", "onOpen(MusicAdapter.java:" + Thread.currentThread()
              .getStackTrace()[2].getLineNumber() + ")" + "position:" + position);
          lastSwipePosition = position;
        }

        @Override public void onStartClose(SwipeLayout layout) {

        }

        @Override public void onClose(SwipeLayout layout) {
        }

        @Override public void onUpdate(SwipeLayout layout, int leftOffset, int topOffset) {

        }

        @Override public void onHandRelease(SwipeLayout layout, float xvel, float yvel) {

        }
      });
    }
    return convertView;
  }

  private class ViewHolder {
    private SwipeLayout slMusic;
    private Button btnDelete;
    private TextView tvName;
  }

  public void setSwipeListener(SwipeListener listener) {
    this.listener = listener;
  }

  public interface SwipeListener {
    void onDelete(int position);
  }

  private void closeByPosition(int position) {
    if (lvMusic != null && list != null && position >= 0 && position < list.size()) {
      int firstVisiblePosition = lvMusic.getFirstVisiblePosition();
      int lastVisiblePosition = lvMusic.getLastVisiblePosition();
      if (position >= firstVisiblePosition && position <= lastVisiblePosition) {
        View view = lvMusic.getChildAt(position - firstVisiblePosition);
        SwipeLayout slMusic = view.findViewById(R.id.sl_music);
        if (slMusic != null) {
          slMusic.close();
        }
      }
    }
  }
}
