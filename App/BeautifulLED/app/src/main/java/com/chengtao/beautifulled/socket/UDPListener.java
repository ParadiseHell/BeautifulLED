package com.chengtao.beautifulled.socket;

/**
 * Created by ChengTao on 2016-11-07.
 */

public interface UDPListener {
  void onInitFail(Exception e);

  void onInitSuccess();

  void onSuccess(int id);

  void onFail(int id, Exception e);

  void onReceive(int id, String response);
}
