package com.chengtao.beautifulled.utils;

/**
 * Created by ChengTao on 2017-02-22.
 */

import android.content.Context;
import android.content.SharedPreferences;

/**
 * SharedPreferences
 */
public class SpUtils {
  private static final String SP_FIRST_IN = "SP_FIRST_IN";
  private static final String FIRST_IN = "FIRST_IN";

  /**
   * is first in or not
   *
   * @param context Context
   * @return true : first in ; false : not first in
   */
  public static boolean isFirstIn(Context context) {
    SharedPreferences sp = context.getSharedPreferences(SP_FIRST_IN, Context.MODE_PRIVATE);
    return sp.getBoolean(FIRST_IN, true);
  }

  /**
   * set not first in
   *
   * @param context Context
   */
  public static void setNotFirstIn(Context context) {
    SharedPreferences sp = context.getSharedPreferences(SP_FIRST_IN, Context.MODE_PRIVATE);
    SharedPreferences.Editor editor = sp.edit();
    editor.putBoolean(FIRST_IN, false);
    editor.apply();
  }
}
