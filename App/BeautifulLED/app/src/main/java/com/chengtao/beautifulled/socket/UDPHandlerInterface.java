package com.chengtao.beautifulled.socket;

import java.io.IOException;

/**
 * Created by ChengTao on 2016-11-07.
 */

public interface UDPHandlerInterface {
    void sendInitSuccessMessage();
    void sendInitFailMessage(Exception e);
    void sendFailMessage(int id,IOException e);
    void sendSuccessMessage(int id);
    void sendReceiveMessage(String msg);
}
