package com.chengtao.beautifulled.database.impl;

import android.database.Cursor;
import android.text.TextUtils;
import android.util.Log;
import com.chengtao.beautifulled.database.SQLiteUtils;
import com.chengtao.beautifulled.database.contract.MusicContract;
import com.chengtao.beautifulled.database.dao.MusicDao;
import com.chengtao.beautifulled.entity.Music;
import java.util.List;

/**
 * Author : ChengTao(chengtaolearn@163.com)
 * Date : 11/12/17
 * Time : 7:39 PM
 * Description :
 */

public class MusicDaoImpl implements MusicDao {
  private SQLiteUtils sqLiteUtils;

  public MusicDaoImpl() {
    sqLiteUtils = SQLiteUtils.getInstance();
  }

  @Override public List<Music> queryAllMusics() {
    return MusicContract.convertCursorToList(sqLiteUtils.query(MusicContract.queryAll()));
  }

  @Override public void insertMusic(String name, String configString) {
    String insertSQL = MusicContract.insert(name, configString);
    if (!TextUtils.isEmpty(insertSQL)) {
      try {
        sqLiteUtils.insert(insertSQL);
      } catch (Throwable throwable) {
        String updateSQL = MusicContract.update(name, configString);
        sqLiteUtils.update(updateSQL);
      }
    }
  }

  @Override public int getMusicCount() {
    Cursor cursor = sqLiteUtils.query(MusicContract.queryCount());
    if (cursor != null && cursor.moveToNext()) {
      String count = cursor.getString(cursor.getColumnIndex(MusicContract.FIELD_COUNT));
      if (!TextUtils.isEmpty(count)) {
        try {
          return Integer.parseInt(count);
        } catch (Exception e) {
          Log.e("TAG", "getMusicCount(MusicDaoImpl.java:" + Thread.currentThread()
              .getStackTrace()[2].getLineNumber() + ")" + "e:" + e.getMessage());
        }
      }
    }
    return 0;
  }
}
