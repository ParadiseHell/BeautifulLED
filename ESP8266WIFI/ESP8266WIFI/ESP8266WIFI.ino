/*
Name:		ESP8266WIFI.ino
Created:	2016/10/28 23:03:49
Author:	ChengTao
*/
//----------����
#include <Wire.h>
#include <EEPROM.h>
#define PRINT_SERIAL Serial//���Դ���
#define PRINT_SERIAL_PORT 9600//���Դ��ڶ˿�
#define ESP8266_SERIAL Serial1//WIFIģ�鴮��
#define ESP8266_SERIAL_PORT 115200//WIFIģ�鴮�ڶ˿�
//-----------��ʼ��
#define INIT_DELAY_TIME 5000

//----------��Ӧ����
char responseBuffer[100] = { '\0' };
char responseChar = '\0';
int responsePos = 0;
String responseStr = "";
//---------��������
/*
�ͻ���ָ���ʽ: CDC + ָ�����ͣ�1,2,3...�� + : + ����
*/
#define DATA_COMMAND_SSID 1//WIFI����
#define DATA_COMMAND_SSID_PASSWORD 2//WIFI����
#define DATA_COMMAND_IP 3//WIFIģ��̶�IP
#define DATA_COMMAND_MASK 4//·������������
#define DATA_COMMAND_GATE 5//·��������
#define DATA_COMMAND_INIT_WIFI 6//����������Ϣ
#define DATA_COMMAND_LED_POS 7//LEDλ��
#define DATA_STOP_FLAG '#'
int dataStyle = 0;
int commondStyle = 0;
bool isDataStart = false;
int ledPos = 0;
//-----------ָ��
#define COMMAND_EXCUTE_MAX_TIME 8000
long cmdStartTime = 0;
String cmd = "";
String cmdSuccess = "";
boolean isCmdSuccess = false;
//--------------����
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
//----------�쳣����
#define WIFI_DISCONNECT "GO IP"

void setup() {
	initAll();
}

void loop() {
	listen();
}

/*
��ʼ��
*/
void initAll() {
	//��ʼ������
	pinMode(13, OUTPUT);
	PRINT_SERIAL.begin(PRINT_SERIAL_PORT);
	ESP8266_SERIAL.begin(ESP8266_SERIAL_PORT);
	while (!PRINT_SERIAL)
	{

	}
	while (!ESP8266_SERIAL)
	{

	}
	Wire.begin();
	//�ȴ�WIFIģ������
	delay(INIT_DELAY_TIME);
	//��ʼ��WIFIģ��
	if (setWIFIMode())//����WIFIģʽ
	{
		if (createWIFI())
		{

		}
		if (createUDPConnection())//����UDP����
		{
			if (initDataFromEEPROM())//��ʼ����������
			{

			}
		}
	}
	resetDataBuffer();
}

/*
����ָ��ж�ָ����Ӧ����״̬
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
	return isCmdSuccess;
}

/*
����WIFIģʽ
*/
boolean setWIFIMode() {
	cmd = "AT+CWMODE=3";
	cmdSuccess = "OK";
	resetDataBufferWhileSysytem();
	return sendCmdAndGetResponseStatus();
}

/*
����WIFI�ȵ㣬����û������·����
*/
bool createWIFI() {
	cmd = "AT+CWSAP=\"BeautifulLed\",\"\",1,0";
	cmdSuccess = "OK";
	resetDataBufferWhileSysytem();
	return sendCmdAndGetResponseStatus();
}

/*
����UDP����
*/
boolean createUDPConnection() {
	cmd = "AT+CIPSTART=\"UDP\",\"192.168.4.2\",8080,8080,2";
	cmdSuccess = "CONNECT";
	resetDataBufferWhileSysytem();
	return sendCmdAndGetResponseStatus();
}

/*
����WIFI
*/
bool connectWIFI() {
	cmd = "AT+CWJAP=\"" + ssid + "\",\"" + ssidPassword + "\"";
	cmdSuccess = "CONNECTED";
	resetDataBufferWhileSysytem();
	return sendCmdAndGetResponseStatus();
}

/*
���ù̶�IP
*/
bool setStableIP() {
	cmd = "AT+CIPSTA_CUR=\"" + wifiIP + "\",\"" + wifiGate + "\",\"" + wifiMask + "\"";
	cmdSuccess = "OK";
	resetDataBufferWhileSysytem();
	return sendCmdAndGetResponseStatus();
}

/*
��EEPROM��ȡ����
��һλ��ʾ�Ƿ������� 1:��
֮�������ǣ�
WIFI�˺ţ�WIFI���룬IP��ַ���������룬����
*/
boolean initDataFromEEPROM() {
	if (EEPROM.read(0) == 1)
	{
		if (EEPROM.read(1) != 255)
		{
			eepromDataNum = EEPROM.read(1);
		}
		//WIFI����
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
		//WIFI����
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
		//��������
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
		//����
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
		if (connectWIFI()) {//����WIFI
			setStableIP();//���ù̶�IP
		}
		return true;
	}
	return false;
}

/*
д�����ݵ�EEPROM
*/
void writeDataToEEPROM(int posStart, int posEnd) {
	//�ж��Ƿ��ٴ�д��
	if (EEPROM.read(posStart) != 255)
	{
		isWriteAgain = true;
		resetRangeEEPROM(posStart, posEnd);
	}
	//д������
	for (int i = posStart; i < posEnd; i++)
	{
		eepromChar = responseBuffer[eepromPos + 5];
		if (eepromChar == '\0')
		{
			break;
		}
		PRINT_SERIAL.print(eepromChar);
		EEPROM.write(i, eepromChar);
		eepromPos++;
	}
	PRINT_SERIAL.println();
	//ֻ��5�����ݵ�EEPROM
	if (eepromDataNum < 5) {
		if (!isWriteAgain)//�����ٴ�д��
		{
			eepromDataNum++;
			EEPROM.write(1, eepromDataNum);
		}
	}
	if (eepromDataNum == 5)
	{
		EEPROM.write(0, 1);
	}
	//����EEPROM��������
	eepromChar = '\0';
	eepromPos = 0;
	isWriteAgain = false;
}

/*
����ĳ�������EEPROMֵ
*/
void resetRangeEEPROM(int posStart, int posEnd) {
	for (int j = posStart; j < posEnd; j++)
	{
		EEPROM.write(j, 255);
	}
}

/*
�����ͻ��˺�WIFIģ�鷢������Ϣ
��loop����ִ��
*/
void listen() {
	//���������
	while (ESP8266_SERIAL.available())
	{
		responseChar = (char)ESP8266_SERIAL.read();
		if (responseChar == 32 || responseChar == 19
			|| responseChar == 11 || responseChar == 12
			|| responseChar == 13 || responseChar == 10) {//ȥ�������ַ�����ո�س�

		}
		else {
			//��ȡ��������
			if (dataStyle == 0)//���û�л�ȡ��������
			{
				if (responsePos == 0)
				{
					if (responseChar == '+')
					{
						dataStyle = 1;//�ͻ��˷���
					}
					else
					{
						dataStyle = 2;//WIFIģ���Զ�����
					}
				}
			}
			switch (dataStyle)
			{
			case 1://�ͻ��˷���
				   //�������ݣ�֪�����ܵ�������
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
					commondStyle = (byte)responseBuffer[3] - 48;
					PRINT_SERIAL.println(commondStyle);
					switch (commondStyle)
					{
					case DATA_COMMAND_SSID://WIFI����
						PRINT_SERIAL.print("SSID:");
						PRINT_SERIAL.println(responseBuffer);
						writeDataToEEPROM(EEPROM_SSID, EEPROM_SSID_PASSWORD);
						break;
					case DATA_COMMAND_SSID_PASSWORD://WIFI����
						PRINT_SERIAL.print("SSID_PASSWORD:");
						PRINT_SERIAL.println(responseBuffer);
						writeDataToEEPROM(EEPROM_SSID_PASSWORD, EEPROM_IP);
						break;
					case DATA_COMMAND_IP://IP
						PRINT_SERIAL.print("IP:");
						PRINT_SERIAL.println(responseBuffer);
						writeDataToEEPROM(EEPROM_IP, EEPROM_MASK);
						break;
					case DATA_COMMAND_MASK://��������
						PRINT_SERIAL.print("MASK:");
						PRINT_SERIAL.println(responseBuffer);
						writeDataToEEPROM(EEPROM_MASK, EEPROM_GATE);
						break;
					case DATA_COMMAND_GATE://����
						PRINT_SERIAL.print("GATE:");
						PRINT_SERIAL.println(responseBuffer);
						writeDataToEEPROM(EEPROM_GATE, 100);
						break;
					case DATA_COMMAND_INIT_WIFI://����WIFI
						initDataFromEEPROM();
						break;
					case DATA_COMMAND_LED_POS://LEDλ��
						if (responseBuffer[6] != '\0')//Ϊ��λ��
						{
							ledPos = 10 * ((byte)(responseBuffer[5] - 48))
								+ (byte)(responseBuffer[6] - 48);
						}
						else
						{
							ledPos = (byte)(responseBuffer[5] - 48);
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
			case 2://WIFIģ���Զ�����
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
		if (responseStr.indexOf(WIFI_DISCONNECT) != -1)//WIFI��Ϊ����ԭ��Ͽ�
		{
			connectWIFI();
		}
		resetDataBuffer();
	}
}

/*
�������ݻ�����
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
�������ݻ�������ϵͳָ�
*/
void resetDataBufferWhileSysytem() {
	memset(responseBuffer, '\0', 100);
	responseChar = '\0';
	responsePos = 0;
	isCmdSuccess = false;
}

/*
���WIFIģ����صĻ���
*/
void resetWIFIBuffer() {
	memset(wifiBuffer, '\0', 20);
	wifiPos = 0;
}

/*
�Զ���delay����
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