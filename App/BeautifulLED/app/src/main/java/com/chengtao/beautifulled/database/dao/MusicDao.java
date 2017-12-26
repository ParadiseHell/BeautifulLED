package com.chengtao.beautifulled.database.dao;

import com.chengtao.beautifulled.entity.Music;
import java.util.List;

/**
 * Author : ChengTao(chengtaolearn@163.com)
 * Date : 11/12/17
 * Time : 7:30 PM
 * Description :
 */

public interface MusicDao {
  List<Music> queryAllMusics();

  void insertMusic(String name, String configString);

  void deleteMusicByName(String name);

  int getMusicCount();
}
