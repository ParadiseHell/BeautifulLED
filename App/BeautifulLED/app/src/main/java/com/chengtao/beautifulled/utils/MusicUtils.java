package com.chengtao.beautifulled.utils;

/**
 * Created by ChengTao on 2017-02-20.
 */

import com.chengtao.pianoview.entity.AutoPlayEntity;
import com.chengtao.pianoview.entity.Piano;

import java.util.ArrayList;

/**
 * 音乐工具
 */
public class MusicUtils {
    //短间隔
    private static final long LITTER_STAR_BREAK_SHORT_TIME = 500;
    //长间隔
    private static final long LITTER_STAR_BREAK_LONG_TIME = 1000;

    /**
     * 获取小星星音乐
     * @return 音乐列表
     */
    public static ArrayList<AutoPlayEntity> getLitterStarMusic(){
        ArrayList<AutoPlayEntity> litterStarList = new ArrayList<>();
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,0,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,0,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,4,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,4,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,5,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,5,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,4,LITTER_STAR_BREAK_LONG_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,3,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,3,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,2,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,2,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,1,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,1,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,0,LITTER_STAR_BREAK_LONG_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,4,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,4,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,3,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,3,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,2,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,2,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,1,LITTER_STAR_BREAK_LONG_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,4,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,4,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,3,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,3,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,2,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,2,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,1,LITTER_STAR_BREAK_LONG_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,0,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,0,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,4,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,4,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,5,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,5,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,4,LITTER_STAR_BREAK_LONG_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,3,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,3,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,2,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,2,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,1,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,1,LITTER_STAR_BREAK_SHORT_TIME));
        litterStarList.add(new AutoPlayEntity(Piano.PianoKeyType.WHITE,4,0,LITTER_STAR_BREAK_LONG_TIME));
        return litterStarList;
    }

    public static String getLittleStarName(){
        return "小星星";
    }
}
