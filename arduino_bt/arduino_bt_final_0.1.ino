/*
  blue test: 
  http://www.kccistc.net/
  작성일 : 2024.03.22
  작성자 : IoT 임베디드 KSH
*/

#include <SoftwareSerial.h>
#include <Wire.h>
#include <DHT.h>
#include <MsTimer2.h>
#include <Servo.h> // 서보모터 라이브러리 추가

#define DEBUG
#define CDS_PIN A0
#define DHTPIN 4 

#define DHTTYPE DHT11
#define ARR_CNT 5
#define CMD_SIZE 60

char sendBuf[CMD_SIZE];
char recvId[10] = "JWJ_LIN";  // SQL 저장 클라이이언트 ID
bool timerIsrFlag = false;
unsigned int secCount;
int cds = 0;
float humi;
float temp;
bool cdsFlag = false;
int getSensorTime;
DHT dht(DHTPIN, DHTTYPE);
Servo myservo;  // 서보모터 객체 생성

SoftwareSerial BTSerial(10, 11); // RX ==>BT:TXD, TX ==> BT:RXD
int varValue;
int varValueold;

void setup()
{
#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("setup() start!");
#endif

  BTSerial.begin(9600); // set the data rate for the SoftwareSerial port
  MsTimer2::set(1000, timerIsr); // 1000ms period
  MsTimer2::start();
  dht.begin();
  myservo.attach(9); // 서보모터 핀 연결
}

void loop()
{
  if (BTSerial.available())
    bluetoothEvent();

  varValue = analogRead(A1);  //0~1023
  varValue = map(varValue, 0, 1023, 0, 100);
  if (abs(varValue - varValueold) > 1000)
  {
    varValueold = varValue;
    sprintf(sendBuf, "[%s]MOTOR@%d\n", "JWJ_BT", varValue);
    BTSerial.write(sendBuf);
  }

  if (timerIsrFlag)
  {
    timerIsrFlag = false;
    cds = map(analogRead(CDS_PIN), 0, 1023, 0, 100);
    humi = dht.readHumidity();
    temp = dht.readTemperature();

    if (getSensorTime != 0 && !(secCount % getSensorTime)) {
      sprintf(sendBuf, "[%s]SENSOR@%d@%d@%d\n", recvId, cds, (int)temp, (int)humi);
      BTSerial.write(sendBuf);
    }

    if ((cds >= 50) && cdsFlag)
    {
      cdsFlag = false;
      sprintf(sendBuf, "[%s]CDS@%d\n", recvId, cds);
      BTSerial.write(sendBuf, strlen(sendBuf));
    }
    else if ((cds < 50) && !cdsFlag)
    {
      cdsFlag = true;
      sprintf(sendBuf, "[%s]CDS@%d\n", recvId, cds);
      BTSerial.write(sendBuf, strlen(sendBuf));
    }
  }

#ifdef DEBUG
  if (Serial.available())
    BTSerial.write(Serial.read());
#endif
}

void bluetoothEvent()
{
  int i = 0;
  char * pToken;
  char * pArray[ARR_CNT] = {0};
  char recvBuf[CMD_SIZE] = {0};
  int len = BTSerial.readBytesUntil('\n', recvBuf, sizeof(recvBuf) - 1);

#ifdef DEBUG
  Serial.print("Recv : ");
  Serial.println(recvBuf);
#endif

  pToken = strtok(recvBuf, "[@]");
  while (pToken != NULL)
  {
    pArray[i] =  pToken;
    if (++i >= ARR_CNT)
      break;
    pToken = strtok(NULL, "[@]");
  }

    if (!strcmp(pArray[1], "RAIN") && !strcmp(pArray[2], "ON")) {
      myservo.write(20); // 서보모터 180도로 회전
      delay(5000); // 3초 대기
      myservo.write(110); // 서보모터 원래 위치로 복귀
    sprintf(sendBuf, "[%s]RAIN@ON\n", pArray[0]);
  }
  else if (!strcmp(pArray[1], "MOTOR")) {
    int speed = atoi(pArray[2]);
    speed = map(speed, 0, 100, 0, 255);
    analogWrite(6, speed);
    sprintf(sendBuf, "[JWJ_SQL]SET@%s@%s@%s\n", pArray[1], pArray[2], pArray[0]);
  }
  else if (!strcmp(pArray[1], "GETSENSOR"))
  {
    if (pArray[2] == NULL) {
      getSensorTime = 0;
    } else {
      getSensorTime = atoi(pArray[2]);
      strcpy(recvId, pArray[0]);
    }
    sprintf(sendBuf, "[%s]SENSOR@%d@%d@%d\n", pArray[0], cds, (int)temp, (int)humi);
  }
  else if (!strncmp(pArray[1], " New", 4)) // New Connected
  {
    return ;
  }
  else if (!strncmp(pArray[1], " Alr", 4)) //Already logged
  {
    return ;
  }
  else
  {
    return;
  }

#ifdef DEBUG
  Serial.print("Send : ");
  Serial.print(sendBuf);
#endif
  BTSerial.write(sendBuf);
}

void timerIsr()
{
  timerIsrFlag = true;
  secCount++;
}