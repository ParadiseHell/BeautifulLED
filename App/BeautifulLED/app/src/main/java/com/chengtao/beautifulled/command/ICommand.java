package com.chengtao.beautifulled.command;

/**
 * Created by ChengTao on 2016-11-08.
 */

public abstract class ICommand {
    private String commandContent;
    public void setCommandContent(String commandContent) {
        this.commandContent = commandContent;
    }
    abstract protected CommandType getCommandType();
    @SuppressWarnings({"SpellCheckingInspection", "WeakerAccess"})
    protected enum CommandType{
        WIFISSID,
        WIFIPASSWORD,
        WIFIIP,
        WIFIMASK,
        WIFIGATE,
        WIFIINIT,
        LED
    }
    public String getCommand(){
        switch (getCommandType()){
            case WIFISSID:
                return BeautifulLed.getWifiSSId() + commandContent + BeautifulLed.getCommandEnd();
            case WIFIPASSWORD:
                return BeautifulLed.getWifiPassword() + commandContent + BeautifulLed.getCommandEnd();
            case WIFIIP:
                return BeautifulLed.getWifiIp() + commandContent + BeautifulLed.getCommandEnd();
            case WIFIMASK:
                return BeautifulLed.getWifiMask() + commandContent + BeautifulLed.getCommandEnd();
            case WIFIGATE:
                return BeautifulLed.getWifiGate() + commandContent + BeautifulLed.getCommandEnd();
            case WIFIINIT:
                return BeautifulLed.getWifiInit() + commandContent + BeautifulLed.getCommandEnd();
            case LED:
                return BeautifulLed.getLED() + commandContent + BeautifulLed.getCommandEnd();
        }
        return null;
    }
}
