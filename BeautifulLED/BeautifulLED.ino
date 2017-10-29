/*
 Name:		BeautifulLED.ino
 Created:	2016/9/28 14:23:55
 Author:	ChengTao
*/

// the setup function runs once when you press reset or power the board
#include <Wire.h>
#include <FastSPI_LED2.h>
#include <fastspi.h>
#include <fastpin.h>
#include <controller.h>
#include <clockless.h>

//---------------����--------------
#define LED_IIC_POS 0//IIC��ַ
#define TOTAL_LED_NUMS 82//�ܵƵ���
#define OUTPUT_PORT 8//����˿�
#define TOTAL_SECTION_NUMS 14//һ���ƴ���ͬʱ��ʾ�ĵƶ���
#define DEFAULT_COLOR_LIGHTNESS 2//Ĭ������ƴ�������
#define DEFAULT_DELAY_TIME 50//Ĭ����ͣʱ��

//----------�����ɫ----------------
#define DEFAULT_RANDOM_COLOR_NUMS 10//Ĭ�ϵ������ɫ����

//Ĭ�ϰ�ɫ�ƶ�
#define DEFAULT_WATER_LED_NUMS 12//Ĭ�ϵ���ˮLED�Ƹ���
#define DEFAULT_WATER_LED_COLOR_NUMS 1//Ĭ�ϵ���ˮLED����ɫ����
#define DEFAULT_WATER_LED_OFF_NUMS 1//Ĭ�ϵ���ˮLED��Ϩ�����
#define DEFAULT_WATER_LED_BREAK 9//ÿ��Ĭ�ϰ�ɫ��֮��ļ�϶
#define DEFAULT_WATER_LED_SECTION_NUMS 4//Ĭ�ϰ�ɫ�εĸ���

//Ĭ�������ɫ��
#define DEFAULT_TOATAL_RANDOM_COLOR_NUMS 1//��ɫ����
#define DEFAULT_RANDOM_LED_NUMS 10//LED����
#define DEFAULT_RANDOM_OFF_LED_NUMS 5//����LED����

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
	boolean isNew;//��־λ����¼�õƶ��Ƿ����²�����
	boolean isInitOK;//��־λ����¼�õƶ��Ƿ��ʼ���ɹ�
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
int defaultWaterColorLightnessArray[DEFAULT_WATER_LED_NUMS] = {2,4,6,8,10,12,12,10,8,6,4,2};//Ĭ��������ˮ�ε�����
int defaultWaterColorNums = DEFAULT_WATER_LED_COLOR_NUMS;

//--------------�����ɫ-----------
//�����ɫ����
long randomColors[DEFAULT_RANDOM_COLOR_NUMS] = 
{0xFF0000,0x00FF00,0x0000FF,
0x00FFFF,0xFFFF00,0x660066,
0xFF00CC,0xFFAB00,0x0009FF,
0xFF8000};
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
//�ʺ���ɫ
long rainbowColors[7] = { 0xFF0000,0xFF7F00,0xFFFF00,0x00FF00,0x0000FF,0x4B0082,0x9400D3 };
//struct LEDSection section[DEFAULT_RANDOM_COLOR_NUMS];
//��������ƶ�Ĭ������
long color[DEFAULT_TOATAL_RANDOM_COLOR_NUMS] = { 0 };
int colorLength = DEFAULT_TOATAL_RANDOM_COLOR_NUMS;
int colorNums[DEFAULT_TOATAL_RANDOM_COLOR_NUMS] = { DEFAULT_RANDOM_LED_NUMS };
int ledNums = DEFAULT_RANDOM_LED_NUMS;
int lightness[DEFAULT_RANDOM_LED_NUMS] = {100,100,100,100,100,100,100,100,100,100};
int offLedNums = DEFAULT_RANDOM_OFF_LED_NUMS;

//��־��ɫ�ƶ��ʼһ���Ƿ��ʼ����
boolean isWriteFirstSectionInitOK = false;
//��־����ƶ��ʼһ���Ƿ��ʼ����
boolean isRandomSectionInitOK = false;

int x = 0;

void setup() {
	LED.init();
	//Serial.begin(9600);
	initEveryLEDSection();
	Wire.begin(LED_IIC_POS);
	Wire.onReceive(receiveEvent);
	randomSleep();
	//Wire.onRequest(requestEvent);
}

void receiveEvent(int bytes) {
	while (Wire.available())
	{
		x = Wire.read();
	}
	//Serial.println(x);
	addSection();
}

//void requestEvent()
//{
//	Wire.write("hello world");
//}

void loop() {
	startLED(DEFAULT_DELAY_TIME);
}

/*
��ʼ��ÿ��LED�ƶ�
*/
void initEveryLEDSection() {
	for (int i = 0; i < TOTAL_SECTION_NUMS; i++)
	{
		ledSections[i].currentPositon = 0;
		ledSections[i].isNew = false;
		//��һ�������һ��ΪĬ�ϵ���ˮ�Եƶ�
		if (i < DEFAULT_WATER_LED_SECTION_NUMS)
		{
			if (i > 0)
			{
				ledSections[i].currentPositon = ledSections[i - 1].currentPositon + DEFAULT_WATER_LED_NUMS + DEFAULT_WATER_LED_BREAK;
			}
			if (ledSections[i].currentPositon - DEFAULT_WATER_LED_NUMS -DEFAULT_WATER_LED_BREAK + 1 >= 0)
			{
				ledSections[i].isInitOK = true;
			}
			else
			{
				ledSections[i].isInitOK = false;
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
			case 4:
				ledSections[i].colorArray = color1;
				break;
			case 5:
				ledSections[i].colorArray = color2;
				break;
			case 6:
				ledSections[i].colorArray = color3;
				break;
			case 7:
				ledSections[i].colorArray = color4;
				break;
			case 8:
				ledSections[i].colorArray = color5;
				break;
			case 9:
				ledSections[i].colorArray = color6;
				break;
			case 10:
				ledSections[i].colorArray = color7;
				break;
			case 11:
				ledSections[i].colorArray = color8;
				break;
			case 12:
				ledSections[i].colorArray = color9;
				break;
			case 13:
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
			ledSections[i].isInitOK = false;
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
				//�ж��²����İ�ɫ�ƶ��Ƿ���Գ�ʼ��
				//-----------��ʼ----------------
				if (ledSections[i].isNew)
				{
					if (i < DEFAULT_WATER_LED_SECTION_NUMS)
					{
						int next = i + 1;
						if (next == DEFAULT_WATER_LED_SECTION_NUMS)
						{
							next = 0;
						}
						if (!ledSections[next].isInitOK)
						{
							break;
						}
					}
					else if (i > DEFAULT_WATER_LED_SECTION_NUMS)
					{
						if (ledSections[i - 1].isUsing)
						{
							if (!ledSections[i - 1].isInitOK)
							{
								break;
							}
						}
					}
					ledSections[i].isNew = false;
				}
				//--------------����-------------
				CRGB rgb = getRGBFromColor(ledSections[i].colorArray[j]);
				for (int k = 0; k < ledSections[i].colorNumsArray[j]; k++)//ѭ��ÿ����ɫ�ĳ���
				{
					int position = ledSections[i].currentPositon + throughLEDNums - ledSections[i].ledNums + 1;
					if (position >= 0 && position < TOTAL_LED_NUMS)
					{
						RGB[position].r = rgb.r * ledSections[i].lightnessArray[ledPosition] / 100;
						RGB[position].g = rgb.g * ledSections[i].lightnessArray[ledPosition] / 100;
						RGB[position].b = rgb.b * ledSections[i].lightnessArray[ledPosition] / 100;
						throughLEDNums++;
						ledPosition++;
					}
					else if (position < 0) {
						throughLEDNums++;
						ledPosition++;
					}
				}
			}
			throughLEDNums = 0;
			if (!ledSections[i].isNew)
			{
				ledSections[i].currentPositon += ledSections[i].offLedNums;
			}
			//�жϰ�ɫ�ƶ��Ƿ��ʼ���ɹ�
			//-----------��ʼ--------------
			if (!ledSections[i].isInitOK)
			{
				if (i < DEFAULT_WATER_LED_SECTION_NUMS)
				{
					if (ledSections[i].currentPositon - DEFAULT_WATER_LED_NUMS - DEFAULT_WATER_LED_BREAK + 1 >= 0)
					{
						ledSections[i].isInitOK = true;
					}
				}
				else 
				{
					//Serial.println(ledSections[i].currentPositon);
					if (ledSections[i].currentPositon - ledSections[i].offLedNums -DEFAULT_RANDOM_LED_NUMS>= 0)
					{
						ledSections[i].isInitOK = true;
					}
				}
				
			}
			//----------����---------------
			if (ledSections[i].currentPositon - ledSections[i].ledNums >= TOTAL_LED_NUMS)
			{
				ledSections[i].currentPositon = 0;
				if (i >= DEFAULT_WATER_LED_SECTION_NUMS)
				{
					ledSections[i].isUsing = false;
					ledSections[i].isNew = false;
					ledSections[i].isInitOK = false;
				}
				if (i < DEFAULT_WATER_LED_SECTION_NUMS)
				{
					ledSections[i].isNew = true;
					ledSections[i].isInitOK = false;
				}
			}
		}
		ledPosition = 0;
	}
	showLED();
	delay(delayTime);
}

/*
���˯��
*/
void randomSleep() {
	randomSeed(analogRead(0));
	int randomNum = random(0, 10) * 100;
	delay(randomNum);
}

/*
��ӵƶ�
*/
void addSection(){
	//randomSeed(analogRead(0));
	for (int i = DEFAULT_WATER_LED_SECTION_NUMS; i < TOTAL_SECTION_NUMS; i++)
	{
		if (!ledSections[i].isUsing)
		{
			ledSections[i].isUsing = true;
			//int randomNum = random(0, 10);
			//ledSections[i].colorArray[0] = randomColors[randomNum];
			ledSections[i].colorArray[0] = rainbowColors[LED_IIC_POS];
			ledSections[i].isNew = true;
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
