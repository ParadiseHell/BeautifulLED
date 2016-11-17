package com.chengtao.beautifulled.command;

/**
 * Created by ChengTao on 2016-11-08.
 */

public class LEDPositionCommand extends ICommand{
    @Override
    protected CommandType getCommandType() {
        return CommandType.LED;
    }
}
