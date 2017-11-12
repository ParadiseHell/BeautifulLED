package com.chengtao.beautifulled.database;

import android.content.Context;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import com.chengtao.beautifulled.database.contract.MusicContract;

/**
 * Author : ChengTao(chengtaolearn@163.com)
 * Date : 11/12/17
 * Time : 7:34 PM
 * Description :
 */

public class PianoAuroraDBHelper extends SQLiteOpenHelper {
  private static final String DATABASE_NAME = "piano_aurora.db";
  private static final int DATABASE_VERSION = 1;

  public PianoAuroraDBHelper(Context context) {
    super(context, DATABASE_NAME, null, DATABASE_VERSION);
  }

  @Override public void onCreate(SQLiteDatabase db) {
    db.execSQL(MusicContract.CREATE_TABLE_SQL);
  }

  @Override public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {

  }
}
