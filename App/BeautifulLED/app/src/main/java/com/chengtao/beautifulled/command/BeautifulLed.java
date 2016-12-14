package com.chengtao.beautifulled.command;

/**
 * Created by ChengTao on 2016-11-08.
 */

public class BeautifulLed {
    //-------------------IP
    public static final String WIFI_HOST_IP = "192.168.4.1";
    //-------------------指令
    private static final String COMMAND_PRESS = "BL";
    private static final String COMMAND_END = "#";
    private static final String WIFI_SSID = COMMAND_PRESS + 0 + ":";
    private static final String WIFI_PASSWORD = COMMAND_PRESS + 1+ ":";
    private static final String WIFI_IP = COMMAND_PRESS + 2+ ":";
    private static final String WIFI_MASK = COMMAND_PRESS + 3+ ":";
    private static final String WIFI_GATE = COMMAND_PRESS + 4+ ":";
    private static final String WIFI_INIT = COMMAND_PRESS + 5+ ":";
    private static final String LED = COMMAND_PRESS + 6+ ":";

    public static String getWifiSsid() {
        return WIFI_SSID;
    }

    public static String getWifiPassword() {
        return WIFI_PASSWORD;
    }

    public static String getWifiMask() {
        return WIFI_MASK;
    }

    public static String getWifiIp() {
        return WIFI_IP;
    }

    public static String getWifiGate() {
        return WIFI_GATE;
    }

    public static String getWifiInit() {
        return WIFI_INIT;
    }

    public static String getLED() {
        return LED;
    }

    public static String getCommandEnd() {
        return COMMAND_END;
    }
}
