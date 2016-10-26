/*
 Name:		ESP8266WIFI.ino
 Created:	2016/10/24 20:30:42
 Author:	ChengTao
*/

/*
基于ESP8266WIFI模块的控制
*/
#include <Wire.h>
//-------------串口
#define ESP8266_WIFI Serial1//ESP8266WIFI模块串口
#define DEBUG_SERIAL Serial//测试串口
#define ESP8266_WIFI_PORT 115200//ESP8266串口波特率
#define DEBUG_SERIAL_PORT 9600//测试串口波特率

//----------------------指令
#define DEFAULT_RETRY_TIME 3//指令等待响应次数
#define DEFAULT_WIFI_RETRY_TIME 5//WIFI指令等待响应次数
#define DEFAULT_RETRY_DELAY_TIME 2000//指令等待响应的延迟时间
#define DEFAULT_COMOND_SHORT_DELAY_TIME 70//指令发送后延迟时间（短）
#define DEFAULT_COMOND_LONG_DELAY_TIME 2000//指令发送后延迟时间（长）

//--------------数据
#define DATA_LED_ARRAY "LED_ARRAY"//客户端发送初始化LED数组特征字符串
#define DATA_LED_POS "LED_POS"//客户端发送指定位置LED闪烁的特征字符串

//--------------模块
String esp8266Ips[2] = { "","" };//ESP8266WIFI模块ip地址
String recentClientIp = "";//最近连接的客户端ip地址

//--------------------程序
#define DEFAULT_LOOP_DELAY_TIME 20//loop循环延迟时间
#define DEFAULT_ESP8266_BREAK_STR "GOT IP"//模块出现断开的特征字符

//----------------WIFI信息
#define SSID "Develop"//WIFI名字
#define SSID_PASSWORD "ccbfudevelop"//WIFI密码

void setup() {
	initESP8266();
	Wire.begin();
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
	delay(5000);
	if (setWIFIMode())
	{
		connectWIFI();
		getESP8266IPs();
		setMultiConnection();
		createUDPChangeableConnection();
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
	response.trim();
	response.toUpperCase();
}

/*
在调试串口打印数据
*/
void printInfo(String response) {
	DEBUG_SERIAL.println(response.c_str());
}

/*
设置WIFI模式，默认为3
*/
boolean setWIFIMode() {
	String response = "";
	int retryTime = 0;
	ESP8266_WIFI.println("AT+CWMODE=3");
	ESP8266_WIFI.flush();
	delay(DEFAULT_COMOND_SHORT_DELAY_TIME);
	//
	return retryRecieveResponse(DEFAULT_RETRY_TIME, DEFAULT_RETRY_DELAY_TIME,
		response, "OK", "setWIFIMode---");
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
	return retryRecieveResponse(DEFAULT_WIFI_RETRY_TIME, DEFAULT_RETRY_DELAY_TIME,
		response, "OK", "connectWIFI---");
}

/*
查看本机IP
*/
boolean getESP8266IPs() {
	String response = "";
	ESP8266_WIFI.println("AT+CIFSR");
	ESP8266_WIFI.flush();
	delay(DEFAULT_COMOND_SHORT_DELAY_TIME);
	//
	return retryRecieveResponse(DEFAULT_RETRY_TIME, DEFAULT_RETRY_DELAY_TIME,
		response, "OK", "getESP8266IPs---");
}

/*
创建多连接
*/
boolean setMultiConnection() {
	String response = "";
	ESP8266_WIFI.println("AT+CIPMUX=0");
	ESP8266_WIFI.flush();
	delay(DEFAULT_COMOND_SHORT_DELAY_TIME);
	//
	return retryRecieveResponse(DEFAULT_RETRY_TIME, DEFAULT_RETRY_DELAY_TIME,
		response, "OK", "setMultiConnection---");
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
	return retryRecieveResponse(DEFAULT_RETRY_TIME, DEFAULT_RETRY_DELAY_TIME,
		response, "CONNECT", "createUDPChangeableConnection---");
}

/*
断开 UDP 通信
*/
boolean disconnectUDP() {
	String response = "";
	ESP8266_WIFI.println("AT+CIPCLOSE");
	ESP8266_WIFI.flush();
	delay(DEFAULT_COMOND_LONG_DELAY_TIME);
	//
	return retryRecieveResponse(DEFAULT_RETRY_TIME, DEFAULT_RETRY_DELAY_TIME,
		response, "CLOSED", "disconnectUDP---");
}

/*
反复接受响应,并返回是否成功
*/
boolean retryRecieveResponse(int retryTime,int delayTime,String response,String specialStr,String methodStr) {
	int rt = 0;
	getData(response);
	while (response.indexOf(specialStr) == -1)
	{
		delay(delayTime);
		rt++;
		getData(response);
		if (rt > retryTime)
		{
			break;
		}
	}
	//
	printInfo(response);
	if (response.indexOf(specialStr) != -1)
	{
		//printInfo(response);
		printInfo(methodStr+"success");
		return true;
	}
	else {
		printInfo(methodStr + "fail");
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
		DEBUG_SERIAL.println(response);
		if (response.indexOf(DATA_LED_POS) != -1)//客服端发送指定灯带闪烁
		{
			String pos = response.substring(response.indexOf(DATA_LED_POS) + 7);
			int p = pos.toInt();
			DEBUG_SERIAL.println(p);
			Wire.beginTransmission(p);
			Wire.write(1);
			Wire.endTransmission();
		}
		if (response.indexOf(DATA_LED_ARRAY) != -1)//客服端发送灯带数组数据
		{

		}
		if (response.indexOf(DEFAULT_ESP8266_BREAK_STR) != -1)//模块断开提示
		{
			setWIFIMode();
			setMultiConnection();
			createUDPChangeableConnection();
		}
	}
	delay(DEFAULT_LOOP_DELAY_TIME);
}

