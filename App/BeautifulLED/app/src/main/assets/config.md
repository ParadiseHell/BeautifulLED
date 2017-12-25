# 如何编写音乐配置文件

## 示例
<pre>
  <code>
    {
    	tune:C;
    	frequency:480;
    	name:小星星;
    }
    1,1,5,5  | 6,6,5*2  | 4,4,3,3  | 2,2,1*2|
    5,5,4,4  | 3,3,2*2  | 5,5,4,4  | 3,3,2*2|
    1,1,5,5  | 6,6,5*2  | 4,4,3,3  | 2,2,1*2|
  </code>
</pre>

## 说明

#### 配置文件头部
- tune : 乐谱属于什么调 ( 默认C调 )
- frequency : 基音的时长 , 单位毫秒 ( 建议乐谱中时长最短的 )
- name : 乐谱名称 ( 必须 )
- **注** : 每个属性后面必须有 ';' ( 分号 ) 分割 , 属性部分必须在 '{}' ( 花括号 ) 之间

##### 对应关系
- 0 : 休止符(乐谱中没有音符的部分)
- 1 : do
- 2 : re
- 3 : mi
- 4 : fa
- 5 : so
- 6 : la
- 7 : si
- 如果一个音的时长是基音时长的好几倍 , 使用 '*' ( 乘号 ) 加上倍数 ; 示例 1\*2
- 如果是升调的音 ( 简谱中数字前面有一个 # 号 ) , 使用 H + 数字 ; 示例 H3
-　如果是还原的音 ( 简谱中数字前面有一个类似闪电的符号 ) , 使用 M + 数字 ; 示例 M3
-　如果是降调的音 ( 简谱中数字前面有一个类似 6 的符号 ) , 使用 L + 数字 ; 示例 L3
- 如果是高8度的音 ( 简谱中数字上方有一点 ) , 使用 HO + 数字 ; 示例 HO3
- 如果是低8度的音 ( 简谱中数字下方有一点 ) , 使用 LO + 数字 ; 示例 LO3
- 如果是高8度升调的音 ( 简谱中数字前面有一个 # 号并且上方有一点 ) , 使用 HOH + 数字 ; 示例 HOH3
- 如果是高8度还原的音 ( 简谱中数字前面有一个类似闪电的符号并且上方有一点 ) , 使用 HOM + 数字 ; 示例 HOM3
- 如果是高8度降调的音 ( 简谱中数字前面有一个类似 6 的符号并且上方有一点 ) , 使用 HOL + 数字 ; 示例 HOL3
- 如果是低8度升调的音 ( 简谱中数字前面有一个 # 号并且下方有一点 ) , 使用 LOH + 数字 ; 示例 LOH3
- 如果是低8度还原的音 ( 简谱中数字前面有一个类似闪电的符号并且下方有一点 ) , 使用 LOM + 数字 ; 示例 LOM3
- 如果是低8度降调的音 ( 简谱中数字前面有一个类似 6 的符号并且下方有一点 ) , 使用 LOL + 数字 ; 示例 LOL3

##### 其他
- 每个音符 ( 简谱对应每个数字 ) 之间用 ',' ( 逗号 ) 分割
- 每一小节用 '|' ( 竖线 ) 分割

## 详细示例
<pre>
  <code>
    {
      tune:C;
      frequency:100;
      name:野蜂飞舞;
    }
    
    0*8                                      |  HO3,HOH2,HOM2,HOH1,  HOM1,HO4,HO3,HOH2   |
    HO3,HOH2,HOM2,HOH1,  HOM1,HOH1,HO2,HOH2  |  HO3,HOH2,HOM2,HOH1,  HOM1,HO4,HO3,HOH2   |
    HO3,HOH2,HOM2,HOH1,  HOM1,HOH1,HO2,HOH2  |  HO3,HOH2,HOM2,HOH1,  HO2,HO1,HOM1,7      |
    HO1,HOH1,HO2,HOH2,   HO3,HO4,HO3*2       |  HO3,HOH2,HOM2,HOH1,  HO2,HO1,HOM1,7      |
    HO1,HOH1,HO2,HOH2,   HO3,HOH4,HO5*2      |  HO6,HOH5,HOM5,HOH4,  HOM4,HOL7,HO6,HOH5  |
    HO6,HOH5,HOM5,HOH4,  HOM4,HOH4,HO5,HOH5  |  HO6,HOH5,HOM5,HOH4,  HOM4,HOL7,HO6,HOH5  |
    HO6,HOH5,HOM5,HOH4,  HOM4,HOH4,HO5,HOH5  |  HO6,HOH5,HOM5,HOH4,  HO5,HO4,HOM4,HO3    |
    HO4,HOH4,HO5,HOH5,   HO6,HOL7,HO6*2      |  HO6,HOH5,HOM5,HOH4,  HO3,HO2,HOM2,HO1    |
    HO4,HOH4,HO5,HOH5,   HO6,HOL7,HO6*2      |  0*8                                      |
    LO6,LOL7,LO6,LOH5,   LO6,LO7,LO6,LO5     |  LO6,LOL7,LO6,LOH5,   LO6,LO7,LO6,LO5     |
    LO6,LO7,LOM7,1,      L2,1,LO7,LOL6       |  LO6,LOL7,LOM7,1,     H1,2,H2,3           |
    4*2,0*2,             0*4                 |  0*8                                      |
    2,L3,2,H1,           2,3,2,1             |  2,L3,2,H1,           2,3,2,1             |
    2,L3,M3,4,           H4,M4,3,4           |  H4,5,H5,6,           H6,7,HO1,HOH1       |
    HO2,HOH1,HOM1,7,     L7,HOL3,HO2,HOH1    |  HO2,HOH1,HOM1,7,     L7,M7,HO1,HOH1      |
    HO2,HOH1,HOM1,7,     HO1,7,L7,6          |  L7,M7,HO1,HOH1,      HOM1,HOH1,HO2,HOH2  |
    HO3,HOH2,HOM2,HOH1,  HO2,HO1,HOM1,7      |  HO1,7,L7,6,          L6,5,H4,M4          |
    3,4,3,H2,            3,4,3,2             |  3,4,3,H2,            3,4,3,2             |
    3,4,3,H2,            3,4,3,2             |  3,4,3,H2,            3,4,3,2             |
    3*2,0*6                                  |  0*8                                      |
    HO4,HOH2,HOM2,HO1,   HOM1,HO4,HO3,HOH2   |  HO3,HOH2,HOM2,HOH1,  HOM1,HOH1,HO2,HOH2  |
    HO3,HOH2,HOM2,HOH1,  HOM1,HO4,HO3,HOH2   |  HO3,HOH2,HOM2,HOH1,  HOM1,HOH1,HO2,HOH2  |
    HO3*2,H5,6,          L7,H7,HO1,HOH1      |  HO2,HOH1,HOM1,7,     HO1,7,L7,6          |
    H5,6,L7,M7,          HO1,HOH1,HO2,HOH2   |  HO3,HOM4,HO3,HOH2,   HO3,HOH4,HO5,HOH5   |
    HO6,HOH5,HOM5,HOH4,  HO5,HO4,HOM4,HO3    |  HO4,HO3,HOH2,HOM2,   HOH1,HOM1,7,L7      |
    6,H5,M5,H4,          5,4,M4,3            |  4,3,H2,M2,           H1,M1,LO7,LOL7      |
    LO6,LOL7,LO6,LOH5,   LO6,LO7,LO6,LO5     |  LO6,LOL7,LO6,LOH5,   LO6,LO7,1,2         |
    3,4,3,H2,            3,4,3,2             |  3,4,3,H2,            3,H4,5,H5           |
    6*2,0*2,             0*4                 |  3*2,0*2,             0*4                 |
    6*2,0*2,             0*4                 |  0*8
  </code>
</pre>