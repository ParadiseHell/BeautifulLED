/*
Name:		ESP8266WIFI.ino
Created:	2016/10/28 23:03:49
Author:	ChengTao
*/
//----------串口
#include <Wire.h>
#include <EEPROM.h>
#define PRINT_SERIAL Serial//调试串口
#define PRINT_SERIAL_PORT 9600//调试串口端口
#define ESP8266_SERIAL Serial1//WIFI模块串口
#define ESP8266_SERIAL_PORT 115200//WIFI模块串口端口
//-----------初始化
#define INIT_DELAY_TIME 10000

//----------响应数据
char responseBuffer[100] = { '\0' };
char responseChar = '\0';
int responsePos = 0;
String responseStr = "";
//---------数据特征
/*
客户端指令格式: BL + 指令类型（0,1,2,3...） + : + 数据 + #
*/
#define COMMAND_FONT_LENGTH 2
#define DATA_COMMAND_SSID 0//WIFI名称
#define DATA_COMMAND_SSID_PASSWORD 1//WIFI密码
#define DATA_COMMAND_IP 2//WIFI模块固定IP
#define DATA_COMMAND_MASK 3//路由器子网掩码
#define DATA_COMMAND_GATE 4//路由器网关
#define DATA_COMMAND_INIT_WIFI 5//设置网络信息
#define DATA_COMMAND_LED_POS 6//LED位置
#define DATA_STOP_FLAG '#'
int dataStyle = 0;
int commondStyle = 0;
bool isDataStart = false;
int ledPos = 0;
//-----------指令
#define COMMAND_EXCUTE_MAX_TIME 8000
long cmdStartTime = 0;
String cmd = "";
String cmdSuccess = "";
boolean isCmdSuccess = false;
//--------------网络
char wifiBuffer[20] = { '\0' };
int wifiPos = 0;
char wifiChar = '\0';
String ssid = "";
String ssidPassword = "";
String wifiIP = "";
String wifiMask = "";
String wifiGate = "";
//------------EEPROM
int eepromPos = 0;
char eepromChar = '\0';
int eepromDataNum = 0;
bool isWriteAgain = false;
#define EEPROM_SSID 10
#define EEPROM_SSID_PASSWORD 25
#define EEPROM_IP 40
#define EEPROM_MASK 55
#define EEPROM_GATE 70
//----------异常问题
#define WIFI_DISCONNECT "GO IP"
//----------发送信息
int messageLength = 0;

void setup() {
	initAll();
}

void loop() {
	listen();
}

/*
初始化
*/
void initAll() {
	//初始化串口
	pinMode(13, OUTPUT);
	PRINT_SERIAL.begin(PRINT_SERIAL_PORT);
	ESP8266_SERIAL.begin(ESP8266_SERIAL_PORT);
	while (!ESP8266_SERIAL)
	{

	}
	Wire.begin();
	digitalWrite(13,LOW);
	//等待WIFI模块启动
	delay(INIT_DELAY_TIME);
	while (ESP8266_SERIAL.available())
	{
		ESP8266_SERIAL.read();
	}
	//初始化WIFI模块
	if (setWIFIMode())//设置WIFI模式
	{
		if (createWIFI())
		{

		}
		if (createUDPConnection())//创建UDP连接
		{
			UPDSuccess();
			//if (initDataFromEEPROM())//初始化网络数据
			//{

			//}
		}
	}
	resetDataBuffer();
}

/*
发送指令并判断指令响应姐结果状态
*/
boolean sendCmdAndGetResponseStatus() {
	ESP8266_SERIAL.println(cmd);
	ESP8266_SERIAL.flush();
	cmdStartTime = millis();
	while (true)
	{
		while (ESP8266_SERIAL.available())
		{
			responseBuffer[responsePos] = ESP8266_SERIAL.read();
			responsePos++;
		}
		responseStr = responseBuffer;
		if (responseStr.indexOf(cmdSuccess) != -1)
		{
			isCmdSuccess = true;
			break;
		}
		if (millis() - cmdStartTime > COMMAND_EXCUTE_MAX_TIME)
		{
			break;
		}
	}
	PRINT_SERIAL.println(responseStr);
	if (isCmdSuccess)
	{
		PRINT_SERIAL.println("T");
	}
	else
	{
		PRINT_SERIAL.println("F");
	}
	myDelay(200);
	return isCmdSuccess;
}

/*
设置WIFI模式
*/
boolean setWIFIMode() {
	cmd = "AT+CWMODE=3";
	cmdSuccess = "OK";
	resetDataBufferWhileSysytem();
	return sendCmdAndGetResponseStatus();
}

/*
创建WIFI热点，用于没有连接路由器
*/
bool createWIFI() {
	cmd = "AT+CWSAP=\"BeautifulLed\",\"\",1,0";
	cmdSuccess = "OK";
	resetDataBufferWhileSysytem();
	return sendCmdAndGetResponseStatus();
}

/*
创建UDP连接
*/
boolean createUDPConnection() {
	cmd = "AT+CIPSTART=\"UDP\",\"192.168.4.2\",8080,8080,2";
	cmdSuccess = "CONNECT";
	resetDataBufferWhileSysytem();
	return sendCmdAndGetResponseStatus();
}

/*
连接WIFI
*/
bool connectWIFI() {
	cmd = "AT+CWJAP=\"" + ssid + "\",\"" + ssidPassword + "\"";
	cmdSuccess = "CONNECTED";
	resetDataBufferWhileSysytem();
	return sendCmdAndGetResponseStatus();
}

/*
设置固定IP
*/
bool setStableIP() {
	cmd = "AT+CIPSTA_CUR=\"" + wifiIP + "\",\"" + wifiGate + "\",\"" + wifiMask + "\"";
	cmdSuccess = "OK";
	resetDataBufferWhileSysytem();
	return sendCmdAndGetResponseStatus();
}

/*
从EEPROM读取数据
第一位表示是否有数据 1:有
之后依次是：
WIFI账号，WIFI密码，IP地址，子网掩码，网关
*/
boolean initDataFromEEPROM() {
	if (EEPROM.read(0) == 1)
	{
		if (EEPROM.read(1) != 255)
		{
			eepromDataNum = EEPROM.read(1);
		}
		//WIFI名称
		for (int i = EEPROM_SSID; i < EEPROM_SSID_PASSWORD; i++)
		{
			if (EEPROM.read(i) == 255)
			{
				break;
			}
			wifiChar = (char)EEPROM.read(i);
			wifiBuffer[wifiPos] = EEPROM.read(i);
			wifiPos++;
		}
		PRINT_SERIAL.print("SSID:");
		PRINT_SERIAL.println(wifiBuffer);
		ssid = wifiBuffer;
		resetWIFIBuffer();
		//WIFI密码
		for (int i = EEPROM_SSID_PASSWORD; i < EEPROM_IP; i++)
		{
			if (EEPROM.read(i) == 255)
			{
				break;
			}
			wifiChar = (char)EEPROM.read(i);
			wifiBuffer[wifiPos] = EEPROM.read(i);
			wifiPos++;
		}
		PRINT_SERIAL.print("SSID_PASSWORD:");
		PRINT_SERIAL.println(wifiBuffer);
		ssidPassword = wifiBuffer;
		resetWIFIBuffer();
		//IP
		for (int i = EEPROM_IP; i < EEPROM_MASK; i++)
		{
			if (EEPROM.read(i) == 255)
			{
				break;
			}
			wifiChar = (char)EEPROM.read(i);
			wifiBuffer[wifiPos] = EEPROM.read(i);
			wifiPos++;
		}
		PRINT_SERIAL.print("IP:");
		PRINT_SERIAL.println(wifiBuffer);
		wifiIP = wifiBuffer;
		resetWIFIBuffer();
		//子网掩码
		for (int i = EEPROM_MASK; i < EEPROM_GATE; i++)
		{
			if (EEPROM.read(i) == 255)
			{
				break;
			}
			wifiChar = (char)EEPROM.read(i);
			wifiBuffer[wifiPos] = EEPROM.read(i);
			wifiPos++;
		}
		PRINT_SERIAL.print("MASK:");
		PRINT_SERIAL.println(wifiBuffer);
		wifiMask = wifiBuffer;
		resetWIFIBuffer();
		//网关
		for (int i = EEPROM_GATE; i < 100; i++)
		{
			if (EEPROM.read(i) == 255)
			{
				break;
			}
			wifiChar = (char)EEPROM.read(i);
			wifiBuffer[wifiPos] = EEPROM.read(i);
			wifiPos++;
		}
		PRINT_SERIAL.print("GATE:");
		PRINT_SERIAL.println(wifiBuffer);
		wifiGate = wifiBuffer;
		resetWIFIBuffer();
		if (connectWIFI()) {//连接WIFI
			setStableIP();//设置固定IP
		}
		return true;
	}
	return false;
}

/*
写入数据到EEPROM
*/
void writeDataToEEPROM(int posStart, int posEnd) {
	//判断是否再次写入
	if (EEPROM.read(posStart) != 255)
	{
		isWriteAgain = true;
		resetRangeEEPROM(posStart, posEnd);
	}
	//写入数据
	for (int i = posStart; i < posEnd; i++)
	{
		eepromChar = responseBuffer[eepromPos + COMMAND_FONT_LENGTH + 2];
		if (eepromChar == '\0')
		{
			break;
		}
		PRINT_SERIAL.print(eepromChar);
		EEPROM.write(i, eepromChar);
		eepromPos++;
	}
	PRINT_SERIAL.println();
	//只存5组数据到EEPROM
	if (eepromDataNum < 5) {
		if (!isWriteAgain)//不是再次写入
		{
			eepromDataNum++;
			EEPROM.write(1, eepromDataNum);
		}
	}
	if (eepromDataNum == 5)
	{
		EEPROM.write(0, 1);
	}
	//重置EEPROM附带数据
	eepromChar = '\0';
	eepromPos = 0;
	isWriteAgain = false;
}

/*
重置某个区间的EEPROM值
*/
void resetRangeEEPROM(int posStart, int posEnd) {
	for (int j = posStart; j < posEnd; j++)
	{
		EEPROM.write(j, 255);
	}
}

/*
监听客户端和WIFI模块发出的消息
在loop里面执行
*/
void listen() {
	//如果有数据
	while (ESP8266_SERIAL.available())
	{
		responseChar = (char)ESP8266_SERIAL.read();
		if (responseChar == 32 || responseChar == 19
			|| responseChar == 11 || responseChar == 12
			|| responseChar == 13 || responseChar == 10) {//去除特殊字符，如空格回车

		}
		else {
			//获取数据类型
			if (dataStyle == 0)//如果没有获取数据类型
			{
				if (responsePos == 0)
				{
					if (responseChar == '+')
					{
						dataStyle = 1;//客户端发出
					}
					else
					{
						dataStyle = 2;//WIFI模块自动发出
					}
				}
			}
			switch (dataStyle)
			{
			case 1://客户端发出
				   //接受数据，知道接受到结束符
				if (responseChar != DATA_STOP_FLAG)
				{
					if (isDataStart)
					{
						responseBuffer[responsePos] = responseChar;
						responsePos++;
					}
					else
					{
						if (responseChar == ':')
						{
							isDataStart = true;
						}
					}
				}
				else
				{
					commondStyle = (byte)responseBuffer[COMMAND_FONT_LENGTH] - 48;
					PRINT_SERIAL.println(commondStyle);
					switch (commondStyle)
					{
					case DATA_COMMAND_SSID://WIFI名称
						PRINT_SERIAL.print("SSID:");
						PRINT_SERIAL.println(responseBuffer);
						writeDataToEEPROM(EEPROM_SSID, EEPROM_SSID_PASSWORD);
						break;
					case DATA_COMMAND_SSID_PASSWORD://WIFI密码
						PRINT_SERIAL.print("SSID_PASSWORD:");
						PRINT_SERIAL.println(responseBuffer);
						writeDataToEEPROM(EEPROM_SSID_PASSWORD, EEPROM_IP);
						break;
					case DATA_COMMAND_IP://IP
						PRINT_SERIAL.print("IP:");
						PRINT_SERIAL.println(responseBuffer);
						writeDataToEEPROM(EEPROM_IP, EEPROM_MASK);
						break;
					case DATA_COMMAND_MASK://子网掩码
						PRINT_SERIAL.print("MASK:");
						PRINT_SERIAL.println(responseBuffer);
						writeDataToEEPROM(EEPROM_MASK, EEPROM_GATE);
						break;
					case DATA_COMMAND_GATE://网关
						PRINT_SERIAL.print("GATE:");
						PRINT_SERIAL.println(responseBuffer);
						writeDataToEEPROM(EEPROM_GATE, 100);
						break;
					case DATA_COMMAND_INIT_WIFI://设置WIFI
						initDataFromEEPROM();
						break;
					case DATA_COMMAND_LED_POS://LED位置
						if (responseBuffer[COMMAND_FONT_LENGTH + 3] != '\0')//为两位数
						{
							ledPos = 10 * ((byte)(responseBuffer[COMMAND_FONT_LENGTH + 2] - 48))
								+ (byte)(responseBuffer[COMMAND_FONT_LENGTH + 3] - 48);
						}
						else
						{
							ledPos = (byte)(responseBuffer[COMMAND_FONT_LENGTH + 2] - 48);
						}
						PRINT_SERIAL.print("LED:");
						PRINT_SERIAL.println(ledPos);
						Wire.beginTransmission(ledPos);
						Wire.write(1);
						Wire.endTransmission(true);
						break;
					}
					resetDataBuffer();
				}
				break;
			case 2://WIFI模块自动发出
				responseBuffer[responsePos] = responseChar;
				responsePos++;
				break;
			}
		}
	}
	if (dataStyle == 2)
	{
		PRINT_SERIAL.println(responseBuffer);
		responseStr = responseBuffer;
		if (responseStr.indexOf(WIFI_DISCONNECT) != -1)//WIFI因为其他原因断开
		{
			connectWIFI();
		}
		resetDataBuffer();
	}
}

/*
重置数据缓冲区
*/
void resetDataBuffer() {
	dataStyle = 0;
	commondStyle = 0;
	isDataStart = false;
	memset(responseBuffer, '\0', 100);
	responseChar = '\0';
	responsePos = 0;
}

/*
重置数据缓冲区（系统指令）
*/
void resetDataBufferWhileSysytem() {
	memset(responseBuffer, '\0', 100);
	responseChar = '\0';
	responsePos = 0;
	isCmdSuccess = false;
}

/*
存放WIFI模块相关的缓冲
*/
void resetWIFIBuffer() {
	memset(wifiBuffer, '\0', 20);
	wifiPos = 0;
}

/*
UDP创建成功标志
*/
void UPDSuccess() {
	for (int i = 0; i < 5; i++)
	{
		digitalWrite(13,HIGH);
		myDelay(200);
		digitalWrite(13, LOW);
	}
}

/*
自定义delay方法
*/
void myDelay(int time) {
	cmdStartTime = millis();
	while (true)
	{
		if (millis() - cmdStartTime > time)
		{
			break;
		}
	}
}

/*
发送消息给其他设备
*/
void sendMessageToDevice(char* message) {
	messageLength = strlen(message);
	cmd = "AT+CIPSEND="+messageLength;
	ESP8266_SERIAL.println(cmd);
	myDelay(70);
	ESP8266_SERIAL.println(message);
}