/*
 Name:		BeautifulLED.ino
 Created:	2016/9/28 14:23:55
 Author:	ChengTao
*/

// the setup function runs once when you press reset or power the board
#include <FastSPI_LED2.h>
#include <fastspi.h>
#include <fastpin.h>
#include <controller.h>
#include <clockless.h>

//---------------常量--------------
#define TOTAL_LED_NUMS 62//总灯点数
#define OUTPUT_PORT 9//输出端口
#define TOTAL_SECTION_NUMS 13//一条灯带能同时显示的灯段数
#define DEFAULT_COLOR_LIGHTNESS 3//默认整体灯带的亮度
#define DEFAULT_DELAY_TIME 60//默认暂停时间

//----------随机颜色----------------
#define DEFAULT_RANDOM_COLOR_NUMS 10//默认的随机颜色个数

//默认白色灯段
#define DEFAULT_WATER_LED_NUMS 12//默认的流水LED灯个数
#define DEFAULT_WATER_LED_COLOR_NUMS 1//默认的流水LED灯颜色个数
#define DEFAULT_WATER_LED_OFF_NUMS 1//默认的流水LED灯熄灭个数
#define DEFAULT_WATER_LED_BREAK 9//每个默认白色段之间的间隙
#define DEFAULT_WATER_LED_SECTION_NUMS 3//默认白色段的个数

//默认随机颜色段
#define DEFAULT_TOATAL_RANDOM_COLOR_NUMS 1//颜色个数
#define DEFAULT_RANDOM_LED_NUMS 6//LED个数
#define DEFAULT_RANDOM_OFF_LED_NUMS 3//灭点的LED个数

//-----------每段的结构体----------
struct LEDSection
{
	int currentPositon;//当前位置
	long* colorArray;//每段的颜色数组
	int colorArrayLength;//每段颜色数组长度
	int* colorNumsArray;//每个颜色的个数的数组
	int offLedNums;//要熄灭的灯点数
	int ledNums;//每段的灯点数
	int* lightnessArray;//亮度数组
	boolean isUsing;//标志位，记录是否在使用
};

//-----------颜色结构体-------------
struct CRGB
{
	byte g;//绿色
	byte r;//红色
	byte b;//蓝色
};

//------------全局变量----------------------
struct LEDSection ledSections[TOTAL_SECTION_NUMS];
struct CRGB RGB[TOTAL_LED_NUMS];
WS2811Controller800Mhz<OUTPUT_PORT> LED;//定义灯带
int static throughLEDNums = 0;//经过的灯点数

//默认情况下白色的灯段参数
long defaultWaterColorArray[DEFAULT_WATER_LED_COLOR_NUMS] = { 0xFFFFFF };//默认颜色数组
int defaultWaterColorNumsArray[DEFAULT_WATER_LED_COLOR_NUMS] = { DEFAULT_WATER_LED_NUMS };//默认每个颜色的个数
int defaultWaterColorLightnessArray[DEFAULT_WATER_LED_NUMS] = {5,10,15,20,25,30,30,25,20,15,10,5};//默认正弦流水段的亮度
int defaultWaterColorNums = DEFAULT_WATER_LED_COLOR_NUMS;

//--------------随机颜色-----------
//随机颜色数组
long randomColors[DEFAULT_RANDOM_COLOR_NUMS] = 
{0xFF0000,0x00FF00,0x0000FF,
0x00FFFF,0xFFFF00,0x660066,
0xFF00CC,0xFFAB00,0x0009FF,
0xFF8000};
//long red[1] = {0xFF0000};
//long green[1] = {0x00FF00};
//long blue[1] = {0x0000FF};
long color1[1] = { 0 };
long color2[1] = { 0 };
long color3[1] = { 0 };
long color4[1] = { 0 };
long color5[1] = { 0 };
long color6[1] = { 0 };
long color7[1] = { 0 };
long color8[1] = { 0 };
long color9[1] = { 0 };
long color10[1] = { 0 };
//struct LEDSection section[DEFAULT_RANDOM_COLOR_NUMS];
//定义随机灯段默认属性
long color[DEFAULT_TOATAL_RANDOM_COLOR_NUMS] = { 0 };
int colorLength = DEFAULT_TOATAL_RANDOM_COLOR_NUMS;
int colorNums[DEFAULT_TOATAL_RANDOM_COLOR_NUMS] = { DEFAULT_RANDOM_LED_NUMS };
int ledNums = DEFAULT_RANDOM_LED_NUMS;
int lightness[DEFAULT_RANDOM_LED_NUMS] = {100,100,100,100,100,100};
int offLedNums = DEFAULT_RANDOM_OFF_LED_NUMS;

int number = 0;

void setup() {
	LED.init();
	Serial.begin(9600);
	initEveryLEDSection();
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (Serial.available() > 0)
	{
		Serial.read();
		addSection();
	}
	startLED(DEFAULT_DELAY_TIME);
}

/*
初始化每个LED灯段
*/
void initEveryLEDSection() {
	for (int i = 0; i < TOTAL_SECTION_NUMS; i++)
	{
		ledSections[i].currentPositon = 0;
		//第一个和最后一个为默认的流水性灯段
		if (i < DEFAULT_WATER_LED_SECTION_NUMS)
		{
			if (i > 0)
			{
				ledSections[i].currentPositon = ledSections[i - 1].currentPositon + DEFAULT_WATER_LED_NUMS + DEFAULT_WATER_LED_BREAK;
			}
			ledSections[i].colorArrayLength = DEFAULT_WATER_LED_COLOR_NUMS;
			ledSections[i].colorArray = defaultWaterColorArray;
			ledSections[i].colorNumsArray = defaultWaterColorNumsArray;
			ledSections[i].offLedNums = DEFAULT_WATER_LED_OFF_NUMS;
			ledSections[i].ledNums = DEFAULT_WATER_LED_NUMS;
			ledSections[i].lightnessArray = defaultWaterColorLightnessArray;
			ledSections[i].isUsing = true;
		}
		else {
			ledSections[i].colorArrayLength = colorLength;
			switch (i)
			{
			case 3:
				ledSections[i].colorArray = color1;
				break;
			case 4:
				ledSections[i].colorArray = color2;
				break;
			case 5:
				ledSections[i].colorArray = color3;
				break;
			case 6:
				ledSections[i].colorArray = color4;
				break;
			case 7:
				ledSections[i].colorArray = color5;
				break;
			case 8:
				ledSections[i].colorArray = color6;
				break;
			case 9:
				ledSections[i].colorArray = color7;
				break;
			case 10:
				ledSections[i].colorArray = color8;
				break;
			case 11:
				ledSections[i].colorArray = color9;
				break;
			case 12:
				ledSections[i].colorArray = color10;
				break;
			default:
				break;
			}
			ledSections[i].colorNumsArray = colorNums;
			ledSections[i].offLedNums = offLedNums;
			ledSections[i].ledNums = ledNums;
			ledSections[i].lightnessArray = lightness;
			ledSections[i].isUsing = false;
		}		
	}
}

/*
启动LED
*/
void startLED(int delayTime) {
	restLEDAndShow();
	static int ledPosition = 0;
	for (int i = 0; i < TOTAL_SECTION_NUMS; i++)
	{
		
		if (ledSections[i].isUsing)//判断是否在使用
		{
			for (int j = ledSections[i].colorArrayLength - 1; j >= 0; j--)//循环颜色数组长度
			{
				CRGB rgb = getRGBFromColor(ledSections[i].colorArray[j]);
				/*if (i > 2)
				{
					Serial.print("color===");
					Serial.print(i);
					Serial.print("===");
					Serial.println(ledSections[i].colorArray[j]);
					Serial.print("r===");
					Serial.println(rgb.r);
					Serial.print("g===");
					Serial.println(rgb.g);
					Serial.print("b===");
					Serial.println(rgb.b);
					Serial.println("done");
				}*/
				for (int k = 0; k < ledSections[i].colorNumsArray[j]; k++)//循环每段颜色的长度
				{
					int position = ledSections[i].currentPositon + throughLEDNums;
					if (position < TOTAL_LED_NUMS)
					{
						RGB[position].r = rgb.r * ledSections[i].lightnessArray[ledPosition] / 100;
						RGB[position].g = rgb.g * ledSections[i].lightnessArray[ledPosition] / 100;
						RGB[position].b = rgb.b * ledSections[i].lightnessArray[ledPosition] / 100;
						throughLEDNums++;
						ledPosition++;
					}
				}
			}
			throughLEDNums = 0;
			ledSections[i].currentPositon += ledSections[i].offLedNums;
			if (ledSections[i].currentPositon >= TOTAL_LED_NUMS)
			{
				ledSections[i].currentPositon = 0;
				if (i >= DEFAULT_WATER_LED_SECTION_NUMS)
				{
					ledSections[i].isUsing = false;
				}
			}
		}
		ledPosition = 0;
	}
	showLED();
	delay(delayTime);
}

/*
添加灯段
*/
void addSection(){
	randomSeed(analogRead(0));
	for (int i = DEFAULT_WATER_LED_SECTION_NUMS; i < TOTAL_SECTION_NUMS; i++)
	{
		if (!ledSections[i].isUsing)
		{
			ledSections[i].isUsing = true;
			int randomNum = random(0, 10);
			ledSections[i].colorArray[0] = randomColors[randomNum];
			break;
		}
	}
}

/*
重置LED每个点
*/
void resetLED() {
	memset((byte*)RGB, DEFAULT_COLOR_LIGHTNESS, TOTAL_LED_NUMS * (sizeof(struct CRGB)));
}

/*
重置每个LED并显示
*/
void restLEDAndShow() {
	resetLED();
	showLED();
}

/*
显示LED
*/
void showLED() {
	LED.showRGB((byte*)RGB, TOTAL_LED_NUMS);
}


/*
获取颜色的RGB值
*/
struct CRGB getRGBFromColor(long color) {
	CRGB rgb;
	rgb.r = (color & 0xFF0000) >> 16;
	rgb.g = (color & 0x00FF00) >> 8;
	rgb.b = (color & 0x0000FF);
	return rgb;
}