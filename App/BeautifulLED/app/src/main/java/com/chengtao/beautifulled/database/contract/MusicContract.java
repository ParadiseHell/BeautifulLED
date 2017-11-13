package com.chengtao.beautifulled.database.contract;

import android.database.Cursor;
import android.text.TextUtils;
import com.chengtao.beautifulled.entity.Music;
import java.util.ArrayList;
import java.util.List;

/**
 * Author : ChengTao(chengtaolearn@163.com)
 * Date : 11/12/17
 * Time : 7:39 PM
 * Description :
 */

public class MusicContract {
  private static final String TABLE_NAME = "music";
  private static final String FIELD_NAME = "name";
  private static final String FIELD_CONFIG_STRING = "config_string";
  public static final String FIELD_COUNT = "count";
  public static final String CREATE_TABLE_SQL = "CREATE TABLE "
      + TABLE_NAME
      + " ( "
      + FIELD_NAME
      + " TEXT PRIMARY KEY , "
      + FIELD_CONFIG_STRING
      + " TEXT"
      + " )";

  public static String insert(String name, String configString) {
    String mName = removeStringStartAndEndQuotes(name);
    String mConfigString = removeStringStartAndEndQuotes(configString);
    if (TextUtils.isEmpty(mName) || TextUtils.isEmpty(mConfigString)) {
      return null;
    }
    return "INSERT INTO "
        + TABLE_NAME
        + " ( "
        + FIELD_NAME
        + " , "
        + FIELD_CONFIG_STRING
        + " ) VALUES ( "
        + "\'"
        + mName
        + "\'"
        + " , "
        + "\'"
        + mConfigString
        + "\'"
        + " )";
  }

  public static String update(String name, String configString) {
    String mName = removeStringStartAndEndQuotes(name);
    String mConfigString = removeStringStartAndEndQuotes(configString);
    if (TextUtils.isEmpty(mName) || TextUtils.isEmpty(mConfigString)) {
      return null;
    }
    return "UPDATE "
        + TABLE_NAME
        + " SET "
        + FIELD_CONFIG_STRING
        + " = "
        + "\'"
        + mConfigString
        + "\'"
        + " WHERE "
        + FIELD_NAME
        + " = "
        + "\'"
        + mName
        + "\'";
  }

  public static String queryAll() {
    return "SELECT * FROM " + TABLE_NAME;
  }

  public static String queryCount() {
    return "SELECT count(*) AS " + FIELD_COUNT + " FROM " + TABLE_NAME;
  }

  public static List<Music> convertCursorToList(Cursor cursor) {
    if (cursor != null && cursor.moveToFirst()) {
      List<Music> list = new ArrayList<>();
      do {
        Music music = new Music();
        music.setName(cursor.getString(cursor.getColumnIndex(FIELD_NAME)));
        music.setConfigString(cursor.getString(cursor.getColumnIndex(FIELD_CONFIG_STRING)));
        list.add(music);
      } while (cursor.moveToNext());
      if (list.size() > 0) {
        return list;
      }
    }
    return null;
  }

  private static String removeStringStartAndEndQuotes(String string) {
    if (!TextUtils.isEmpty(string) && !string.equals("\"") && string.length() > 1) {
      String newString = string;
      if (newString.substring(0, 1).equals("\"")) {
        newString = newString.substring(1);
      }
      if (newString.substring(newString.length() - 1).equals("\"")) {
        newString = newString.substring(0, newString.length() - 1);
      }
      return newString;
    }
    return null;
  }
}
