package com.chengtao.beautifulled;

import android.app.Application;
import android.util.Log;
import com.chengtao.beautifulled.database.SQLiteUtils;

/**
 * Created by ChengTao on 2016-12-14.
 */

public class App extends Application {
  @Override public void onCreate() {
    super.onCreate();
    try {
      SQLiteUtils.getInstance().init(this);
    } catch (Throwable throwable) {
      Log.e("TAG", "onCreate(App.java:"
          + Thread.currentThread().getStackTrace()[2].getLineNumber()
          + ")"
          + "throwable:"
          + throwable.getMessage());
    }
  }

  @Override public void onTerminate() {
    super.onTerminate();
    SQLiteUtils.getInstance().close();
  }
}
