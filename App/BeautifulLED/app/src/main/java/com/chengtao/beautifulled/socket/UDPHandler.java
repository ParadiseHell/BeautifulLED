package com.chengtao.beautifulled.socket;

import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import java.io.IOException;

/**
 * Created by ChengTao on 2016-11-07.
 */

@SuppressWarnings("WeakerAccess") public class UDPHandler implements UDPHandlerInterface {
  //----------------------常量
  private static final int INIT_FAIL = 1;
  private static final int INIT_SUCCESS = 2;
  private static final int SEND_FAIL = 3;
  private static final int SEND_SUCCESS = 4;
  //----------------------变量
  private ResponseHandler mHandler;
  private UDPListener listener;

  public UDPHandler(UDPListener listener) {
    this.listener = listener;
    mHandler = new ResponseHandler(Looper.myLooper(), this);
  }

  @Override public void sendInitSuccessMessage() {
    sendMessage(obtainMessage(INIT_SUCCESS, null));
  }

  @Override public void sendInitFailMessage(Exception e) {
    sendMessage(obtainMessage(INIT_FAIL, new Object[] { e }));
  }

  @Override public void sendFailMessage(int id, IOException e) {
    sendMessage(obtainMessage(SEND_FAIL, new Object[] { id, e }));
  }

  @Override public void sendSuccessMessage(int id) {
    sendMessage(obtainMessage(SEND_SUCCESS, new Object[] { id }));
  }

  @Override public void sendReceiveMessage(String msg) {

  }

  private void handelMessage(Message message) {
    Object response[];
    switch (message.what) {
      case INIT_FAIL:
        response = (Object[]) message.obj;
        if (response != null) {
          listener.onInitFail((Exception) response[1]);
        }
        break;
      case INIT_SUCCESS:
        listener.onInitSuccess();
        break;
      case SEND_SUCCESS:
        response = (Object[]) message.obj;
        if (response != null) {
          listener.onSuccess((Integer) response[0]);
        }
        break;
      case SEND_FAIL:
        response = (Object[]) message.obj;
        if (response != null) {
          listener.onFail((Integer) response[0], (Exception) response[1]);
        }
        break;
    }
  }

  private void sendMessage(Message message) {
    if (mHandler == null) {
      handelMessage(message);
    } else {
      mHandler.sendMessage(message);
    }
  }

  private Message obtainMessage(int id, Object data) {
    return Message.obtain(mHandler, id, data);
  }

  private class ResponseHandler extends Handler {
    private UDPHandler handler;

    ResponseHandler(Looper looper, UDPHandler handler) {
      super(looper);
      this.handler = handler;
    }

    @Override public void handleMessage(Message msg) {
      handler.handelMessage(msg);
    }
  }
}
