package com.chengtao.beautifulled.database;

import android.app.Application;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.text.TextUtils;
import android.util.Log;

/**
 * Author : ChengTao(chengtaolearn@163.com)
 * Date : 11/12/17
 * Time : 7:45 PM
 * Description :
 */

@SuppressWarnings("CaughtExceptionImmediatelyRethrown") public class SQLiteUtils {
  private static SQLiteUtils instance = null;
  private PianoAuroraDBHelper dbHelper;

  private SQLiteUtils() {
  }

  public static SQLiteUtils getInstance() {
    if (instance == null) {
      synchronized (SQLiteUtils.class) {
        if (instance == null) {
          instance = new SQLiteUtils();
        }
      }
    }
    return instance;
  }

  public void insert(String sql) throws Throwable {
    if (!TextUtils.isEmpty(sql) && dbHelper != null) {
      try {
        SQLiteDatabase db = dbHelper.getWritableDatabase();
        db.execSQL(sql);
      } catch (Exception e) {
        throw e;
      }
    }
  }

  public void delete(String sql) throws Throwable {
    if (!TextUtils.isEmpty(sql) && dbHelper != null) {
      try {
        SQLiteDatabase db = dbHelper.getWritableDatabase();
        db.execSQL(sql);
      } catch (Exception e) {
        throw e;
      }
    }
  }

  public void update(String sql) {
    if (!TextUtils.isEmpty(sql) && dbHelper != null) {
      try {
        SQLiteDatabase db = dbHelper.getWritableDatabase();
        db.execSQL(sql);
      } catch (Exception e) {
        Log.e("TAG", "update(SQLiteUtils.java:"
            + Thread.currentThread().getStackTrace()[2].getLineNumber()
            + ")"
            + "e:"
            + e.getMessage());
      }
    }
  }

  public Cursor query(String sql) {
    if (!TextUtils.isEmpty(sql) && dbHelper != null) {
      try {
        SQLiteDatabase db = dbHelper.getWritableDatabase();
        return db.rawQuery(sql, null);
      } catch (Exception e) {
        Log.e("TAG", "update(SQLiteUtils.java:"
            + Thread.currentThread().getStackTrace()[2].getLineNumber()
            + ")"
            + "e:"
            + e.getMessage());
      }
    }
    return null;
  }

  public void init(Context context) throws Throwable {
    if (context instanceof Application) {
      dbHelper = new PianoAuroraDBHelper(context);
    } else {
      throw new IllegalArgumentException("context must be Application");
    }
  }

  public void close() {
    if (dbHelper != null) {
      dbHelper.close();
    }
  }
}
