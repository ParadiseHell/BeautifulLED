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

//---------------����--------------
#define TOTAL_LED_NUMS 62//�ܵƵ���
#define OUTPUT_PORT 9//����˿�
#define TOTAL_SECTION_NUMS 13//һ���ƴ���ͬʱ��ʾ�ĵƶ���
#define DEFAULT_COLOR_LIGHTNESS 3//Ĭ������ƴ�������
#define DEFAULT_DELAY_TIME 60//Ĭ����ͣʱ��

//----------�����ɫ----------------
#define DEFAULT_RANDOM_COLOR_NUMS 10//Ĭ�ϵ������ɫ����

//Ĭ�ϰ�ɫ�ƶ�
#define DEFAULT_WATER_LED_NUMS 12//Ĭ�ϵ���ˮLED�Ƹ���
#define DEFAULT_WATER_LED_COLOR_NUMS 1//Ĭ�ϵ���ˮLED����ɫ����
#define DEFAULT_WATER_LED_OFF_NUMS 1//Ĭ�ϵ���ˮLED��Ϩ�����
#define DEFAULT_WATER_LED_BREAK 9//ÿ��Ĭ�ϰ�ɫ��֮��ļ�϶
#define DEFAULT_WATER_LED_SECTION_NUMS 3//Ĭ�ϰ�ɫ�εĸ���

//Ĭ�������ɫ��
#define DEFAULT_TOATAL_RANDOM_COLOR_NUMS 1//��ɫ����
#define DEFAULT_RANDOM_LED_NUMS 6//LED����
#define DEFAULT_RANDOM_OFF_LED_NUMS 3//����LED����

//-----------ÿ�εĽṹ��----------
struct LEDSection
{
	int currentPositon;//��ǰλ��
	long* colorArray;//ÿ�ε���ɫ����
	int colorArrayLength;//ÿ����ɫ���鳤��
	int* colorNumsArray;//ÿ����ɫ�ĸ���������
	int offLedNums;//ҪϨ��ĵƵ���
	int ledNums;//ÿ�εĵƵ���
	int* lightnessArray;//��������
	boolean isUsing;//��־λ����¼�Ƿ���ʹ��
};

//-----------��ɫ�ṹ��-------------
struct CRGB
{
	byte g;//��ɫ
	byte r;//��ɫ
	byte b;//��ɫ
};

//------------ȫ�ֱ���----------------------
struct LEDSection ledSections[TOTAL_SECTION_NUMS];
struct CRGB RGB[TOTAL_LED_NUMS];
WS2811Controller800Mhz<OUTPUT_PORT> LED;//����ƴ�
int static throughLEDNums = 0;//�����ĵƵ���

//Ĭ������°�ɫ�ĵƶβ���
long defaultWaterColorArray[DEFAULT_WATER_LED_COLOR_NUMS] = { 0xFFFFFF };//Ĭ����ɫ����
int defaultWaterColorNumsArray[DEFAULT_WATER_LED_COLOR_NUMS] = { DEFAULT_WATER_LED_NUMS };//Ĭ��ÿ����ɫ�ĸ���
int defaultWaterColorLightnessArray[DEFAULT_WATER_LED_NUMS] = {5,10,15,20,25,30,30,25,20,15,10,5};//Ĭ��������ˮ�ε�����
int defaultWaterColorNums = DEFAULT_WATER_LED_COLOR_NUMS;

//--------------�����ɫ-----------
//�����ɫ����
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
//��������ƶ�Ĭ������
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
��ʼ��ÿ��LED�ƶ�
*/
void initEveryLEDSection() {
	for (int i = 0; i < TOTAL_SECTION_NUMS; i++)
	{
		ledSections[i].currentPositon = 0;
		//��һ�������һ��ΪĬ�ϵ���ˮ�Եƶ�
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
����LED
*/
void startLED(int delayTime) {
	restLEDAndShow();
	static int ledPosition = 0;
	for (int i = 0; i < TOTAL_SECTION_NUMS; i++)
	{
		
		if (ledSections[i].isUsing)//�ж��Ƿ���ʹ��
		{
			for (int j = ledSections[i].colorArrayLength - 1; j >= 0; j--)//ѭ����ɫ���鳤��
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
				for (int k = 0; k < ledSections[i].colorNumsArray[j]; k++)//ѭ��ÿ����ɫ�ĳ���
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
��ӵƶ�
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
����LEDÿ����
*/
void resetLED() {
	memset((byte*)RGB, DEFAULT_COLOR_LIGHTNESS, TOTAL_LED_NUMS * (sizeof(struct CRGB)));
}

/*
����ÿ��LED����ʾ
*/
void restLEDAndShow() {
	resetLED();
	showLED();
}

/*
��ʾLED
*/
void showLED() {
	LED.showRGB((byte*)RGB, TOTAL_LED_NUMS);
}


/*
��ȡ��ɫ��RGBֵ
*/
struct CRGB getRGBFromColor(long color) {
	CRGB rgb;
	rgb.r = (color & 0xFF0000) >> 16;
	rgb.g = (color & 0x00FF00) >> 8;
	rgb.b = (color & 0x0000FF);
	return rgb;
}