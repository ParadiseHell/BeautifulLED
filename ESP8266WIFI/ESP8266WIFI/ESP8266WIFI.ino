/*
 Name:		ESP8266WIFI.ino
 Created:	2016/10/24 20:30:42
 Author:	ChengTao
*/

/*
基于ESP8266WIFI模块的控制
*/
#define ESP8266_WIFI Serial1//ESP8266WIFI模块串口
#define DEBUG_SERIAL Serial//测试串口
#define ESP8266_WIFI_PORT 115200//ESP8266串口波特率
#define DEBUG_SERIAL_PORT 9600//测试串口波特率

#define SSID "Develop"//WIFI名字
#define SSID_PASSWORD "ccbfudevelop"//WIFI密码

#define DEFAULT_RETRY_TIME 5//重新连接次数

#define DEFAULT_COMOND_SHORT_DELAY_TIME 70//指令发送后延迟时间（短）
#define DEFAULT_COMOND_LONG_DELAY_TIME 2000//指令发送后延迟时间（长）

#define DEFAULT_LOOP_DELAY_TIME 20//循环延迟时间

#define DATA_LED_ARRAY "LED_ARRAY"//客户端发送初始化LED数组特征字符串
#define DATA_LED_POS "LED_POS"//客户端发送指定位置LED闪烁的特征字符串

String esp8266Ips[2] = { "","" };//ESP8266WIFI模块ip地址
String recentClientIp = "";//最近连接的客户端ip地址

void setup() {
	initESP8266();
}

void loop() {
	listenDataFromClient();
}
/*
初始化
*/
void initESP8266() {
	//初始化调试串口
	DEBUG_SERIAL.begin(DEBUG_SERIAL_PORT);
	while (!DEBUG_SERIAL)
	{

	}
	//初始化WIFI连接的硬件串口
	ESP8266_WIFI.begin(ESP8266_WIFI_PORT);
	while (!ESP8266_WIFI)
	{

	}
	delay(2000);
	if (setWIFIMode())
	{
		connectWIFI();
	}
}

/*
反复查看是否有数据返回
*/
void retryUntilData() {
	int time = 0;
	while (ESP8266_WIFI.available() == 0)
	{
		if (time == DEFAULT_RETRY_TIME)
		{
			break;
		}
		time++;
	}
}
/*
获取数据
*/
void getData(String &response) {
	while (ESP8266_WIFI.available())
	{
		response += (char)ESP8266_WIFI.read();
	}
}

/*
在调试串口打印数据
*/
void printResponse(String methodName, String response) {
	DEBUG_SERIAL.print(methodName);
	DEBUG_SERIAL.println(response);
}

/*
设置WIFI模式，默认为3
*/
boolean setWIFIMode() {
	String response = "";
	ESP8266_WIFI.println("AT+CWMODE=3");
	ESP8266_WIFI.flush();
	delay(DEFAULT_COMOND_SHORT_DELAY_TIME);
	//
	retryUntilData();
	//
	getData(response);
	//
	printResponse("setWIFIMode===", response);
	//分析接受的数据
	if (!response.equals(""))
	{
		response.toUpperCase();
		if (response.indexOf("OK") != -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/*
连接WIFI
*/
boolean connectWIFI() {
	String response = "";
	String cmd = "";
	cmd += "AT+CWJAP=\"";
	cmd += SSID;
	cmd += "\",\"";
	cmd += SSID_PASSWORD;
	cmd += "\"";
	ESP8266_WIFI.println(cmd);
	ESP8266_WIFI.flush();
	delay(DEFAULT_COMOND_SHORT_DELAY_TIME);
	//
	retryUntilData();
	//
	getData(response);
	//
	printResponse("connectWIFI===", response);
	//
	if (response != "")
	{
		response.toUpperCase();
		if (response.indexOf("OK") != -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/*
查看本机IP
*/
void getESP8266IPs() {
	String response = "";
	ESP8266_WIFI.println("AT+CIFSR");
	ESP8266_WIFI.flush();
	delay(DEFAULT_COMOND_SHORT_DELAY_TIME);
	//
	retryUntilData();
	//
	getData(response);
	//
	printResponse("getESP8266IPs===", response);
	//
	if (response != "")
	{
		response.toUpperCase();
		response.trim();
	}
}

/*
创建多连接
*/
boolean setMultiConnection() {
	String response = "";
	ESP8266_WIFI.println("AT+CIPMUX=1");
	ESP8266_WIFI.flush();
	delay(DEFAULT_COMOND_SHORT_DELAY_TIME);
	//
	retryUntilData();
	//
	getData(response);
	//
	printResponse("getESP8266IPs===", response);
	//
	if (response != "")
	{
		response.toUpperCase();
		if (response.indexOf("OK") != -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/*
创建远端可变的 UDP 通信
*/
boolean createUDPChangeableConnection() {
	String response = "";
	ESP8266_WIFI.println("AT+CIPSTART=\"UDP\",\"192.168.4.2\",8080,8080,2");
	ESP8266_WIFI.flush();
	delay(DEFAULT_COMOND_LONG_DELAY_TIME);
	//
	retryUntilData();
	//
	getData(response);
	//
	printResponse("createUDPChangeableConnection===", response);
	//
	if (response != "")
	{
		response.toUpperCase();
		if (response.indexOf("OK") != -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

/*
监听从客户端发送的数据
*/
void listenDataFromClient() {
	String response = "";
	while (ESP8266_WIFI.available())
	{
		response += (char)ESP8266_WIFI.read();
	}
	if (response != "")
	{
		response.toUpperCase();
		if (response.indexOf(DATA_LED_POS) != -1)//客服端发送指定灯带闪烁
		{

		}
		if (response.indexOf(DATA_LED_ARRAY) != -1)//客服端发送灯带数组数据
		{

		}
	}
	delay(DEFAULT_LOOP_DELAY_TIME);
}
