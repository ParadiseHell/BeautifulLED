/*
 Name:		ESP8266WIFI.ino
 Created:	2016/10/24 20:30:42
 Author:	ChengTao
*/

/*
����ESP8266WIFIģ��Ŀ���
*/
#include <Wire.h>
//-------------����
#define ESP8266_WIFI Serial1//ESP8266WIFIģ�鴮��
#define DEBUG_SERIAL Serial//���Դ���
#define ESP8266_WIFI_PORT 115200//ESP8266���ڲ�����
#define DEBUG_SERIAL_PORT 9600//���Դ��ڲ�����

//----------------------ָ��
#define DEFAULT_RETRY_TIME 3//ָ��ȴ���Ӧ����
#define DEFAULT_WIFI_RETRY_TIME 5//WIFIָ��ȴ���Ӧ����
#define DEFAULT_RETRY_DELAY_TIME 2000//ָ��ȴ���Ӧ���ӳ�ʱ��
#define DEFAULT_COMOND_SHORT_DELAY_TIME 70//ָ��ͺ��ӳ�ʱ�䣨�̣�
#define DEFAULT_COMOND_LONG_DELAY_TIME 2000//ָ��ͺ��ӳ�ʱ�䣨����

//--------------����
#define DATA_LED_ARRAY "LED_ARRAY"//�ͻ��˷��ͳ�ʼ��LED���������ַ���
#define DATA_LED_POS "LED_POS"//�ͻ��˷���ָ��λ��LED��˸�������ַ���

//--------------ģ��
String esp8266Ips[2] = { "","" };//ESP8266WIFIģ��ip��ַ
String recentClientIp = "";//������ӵĿͻ���ip��ַ

//--------------------����
#define DEFAULT_LOOP_DELAY_TIME 20//loopѭ���ӳ�ʱ��
#define DEFAULT_ESP8266_BREAK_STR "GOT IP"//ģ����ֶϿ��������ַ�

//----------------WIFI��Ϣ
#define SSID "Develop"//WIFI����
#define SSID_PASSWORD "ccbfudevelop"//WIFI����

void setup() {
	initESP8266();
	Wire.begin();
}

void loop() {
	listenDataFromClient();
}
/*
��ʼ��
*/
void initESP8266() {
	//��ʼ�����Դ���
	DEBUG_SERIAL.begin(DEBUG_SERIAL_PORT);
	while (!DEBUG_SERIAL)
	{

	}
	//��ʼ��WIFI���ӵ�Ӳ������
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
��ȡ����
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
�ڵ��Դ��ڴ�ӡ����
*/
void printInfo(String response) {
	DEBUG_SERIAL.println(response.c_str());
}

/*
����WIFIģʽ��Ĭ��Ϊ3
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
����WIFI
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
�鿴����IP
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
����������
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
����Զ�˿ɱ�� UDP ͨ��
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
�Ͽ� UDP ͨ��
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
����������Ӧ,�������Ƿ�ɹ�
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
�����ӿͻ��˷��͵�����
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
		if (response.indexOf(DATA_LED_POS) != -1)//�ͷ��˷���ָ���ƴ���˸
		{
			String pos = response.substring(response.indexOf(DATA_LED_POS) + 7);
			int p = pos.toInt();
			DEBUG_SERIAL.println(p);
			Wire.beginTransmission(p);
			Wire.write(1);
			Wire.endTransmission();
		}
		if (response.indexOf(DATA_LED_ARRAY) != -1)//�ͷ��˷��͵ƴ���������
		{

		}
		if (response.indexOf(DEFAULT_ESP8266_BREAK_STR) != -1)//ģ��Ͽ���ʾ
		{
			setWIFIMode();
			setMultiConnection();
			createUDPChangeableConnection();
		}
	}
	delay(DEFAULT_LOOP_DELAY_TIME);
}

