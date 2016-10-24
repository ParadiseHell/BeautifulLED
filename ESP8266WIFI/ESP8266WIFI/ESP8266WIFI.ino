/*
 Name:		ESP8266WIFI.ino
 Created:	2016/10/24 20:30:42
 Author:	ChengTao
*/

/*
����ESP8266WIFIģ��Ŀ���
*/
#define ESP8266_WIFI Serial1//ESP8266WIFIģ�鴮��
#define DEBUG_SERIAL Serial//���Դ���
#define ESP8266_WIFI_PORT 115200//ESP8266���ڲ�����
#define DEBUG_SERIAL_PORT 9600//���Դ��ڲ�����

#define SSID "Develop"//WIFI����
#define SSID_PASSWORD "ccbfudevelop"//WIFI����

#define DEFAULT_RETRY_TIME 5//�������Ӵ���

#define DEFAULT_COMOND_SHORT_DELAY_TIME 70//ָ��ͺ��ӳ�ʱ�䣨�̣�
#define DEFAULT_COMOND_LONG_DELAY_TIME 2000//ָ��ͺ��ӳ�ʱ�䣨����

#define DEFAULT_LOOP_DELAY_TIME 20//ѭ���ӳ�ʱ��

#define DATA_LED_ARRAY "LED_ARRAY"//�ͻ��˷��ͳ�ʼ��LED���������ַ���
#define DATA_LED_POS "LED_POS"//�ͻ��˷���ָ��λ��LED��˸�������ַ���

String esp8266Ips[2] = { "","" };//ESP8266WIFIģ��ip��ַ
String recentClientIp = "";//������ӵĿͻ���ip��ַ

void setup() {
	initESP8266();
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
	delay(2000);
	if (setWIFIMode())
	{
		connectWIFI();
	}
}

/*
�����鿴�Ƿ������ݷ���
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
��ȡ����
*/
void getData(String &response) {
	while (ESP8266_WIFI.available())
	{
		response += (char)ESP8266_WIFI.read();
	}
}

/*
�ڵ��Դ��ڴ�ӡ����
*/
void printResponse(String methodName, String response) {
	DEBUG_SERIAL.print(methodName);
	DEBUG_SERIAL.println(response);
}

/*
����WIFIģʽ��Ĭ��Ϊ3
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
	//�������ܵ�����
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
�鿴����IP
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
����������
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
����Զ�˿ɱ�� UDP ͨ��
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
		if (response.indexOf(DATA_LED_POS) != -1)//�ͷ��˷���ָ���ƴ���˸
		{

		}
		if (response.indexOf(DATA_LED_ARRAY) != -1)//�ͷ��˷��͵ƴ���������
		{

		}
	}
	delay(DEFAULT_LOOP_DELAY_TIME);
}
