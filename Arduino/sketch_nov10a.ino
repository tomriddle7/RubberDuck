/**************************************************
*        2016학년도 종합설계 프로젝트
*        안전을 지키는 러버덕 프로젝트
*        지도교수 - 심귀보 교수님
*        팀원 - 노준경, 이상진, 하창하, 황금기
**************************************************/
#include <SoftwareSerial.h>

#define FORWARD  1 // 모터 정, 역회전 조정용
#define BACKWARD  0

#define SPEED_100  0xff // 모터 속도 조절용
#define SPEED_80   0xC0
#define SPEED_50   0x80
#define SPEED_30   0x40
#define SPEED_0    0x00

int distPin = 0; // 깊이별 LED 색상 조정
float distVolt, distance, scalingFactor;

int ledPin;
int prevLED = 9;
int redPin = 9;
int yellowPin = 10;
int greenPin = 11;

void forw(); // 러버덕 방향 조정
void forw_l(); 
void forw_r();
void back();
void sstop();

int motor_a_dir = 4;
int motor_a_pwm = 5;
int motor_b_dir = 7;
int motor_b_pwm = 6;

char val;

SoftwareSerial mySerial(0, 1); // RX, TX

void setup()
{
  Serial.begin(115200); // 시리얼 통신을 열고 포트가 열리기를 대기
  mySerial.begin(115200); // SoftwareSerial 포트의 통신 속도 설정

  scalingFactor = 0.886;
}

void loop()
{
  distVolt = analogRead(distPin); // 초음파 센서에서 전달받은 값을 바탕으로 수심 계산
  distance = distVolt * 4.32;
  distance = distance / 2.54;
  distance = distance * scalingFactor;
    
  if (mySerial.available()) // SoftwareSerial 포트 작동 확인 후 수심을 전송
    Serial.println(distance);
  while (Serial.available()) { // 앱에서 보내온 값을 읽어 모터를 동작
    val = Serial.read();
    if (val==0x38) {
      forw();
      delay(1000);
      sstop();
      delay(500);
    }
    if (val==0x32) {
      back();
      delay(1000);
      sstop();
      delay(500);
    }
    if (val==0x34) {
      forw_l();
      delay(500);
      sstop();
      delay(500);
    }
    if (val==0x36){
      forw_r();
      delay(500);
      sstop();
      delay(500);
    }
  }
// 0x38 - 직진
// 0x32 - 후진
// 0x34 - 좌회전
// 0x36 - 우회전
  if(distance > 10*12) { // 수심이 10미터 초과일 경우 녹색 LED 점등
    ledPin = greenPin;
  }
  else if(distance > 6*12) { // 수심이 6미터 초과 10미터 미만일 경우 황색 LED 점등
    ledPin = yellowPin;
  }
  else { // 수심이 6미터 미만일 경우 적색 LED 점등
    ledPin = redPin;
  }
  digitalWrite(prevLED, LOW);

  analogWrite(ledPin, 255); // 점등하는 LED의 밝기 설정
  prevLED = ledPin;

  delay(1000);
}
// 수신하는 값에 따른 모터 동작 방향 선택
void forw() // 직진
{
  digitalWrite(motor_a_dir, FORWARD); analogWrite(motor_a_pwm, SPEED_80);
  digitalWrite(motor_b_dir, BACKWARD); analogWrite(motor_b_pwm, SPEED_80);
}

void forw_l() // 좌회전
{
  digitalWrite(motor_a_dir, FORWARD); analogWrite(motor_a_pwm, SPEED_80);
  digitalWrite(motor_b_dir, BACKWARD); analogWrite(motor_b_pwm, SPEED_0);
}

void forw_r() // 우회전
{
  digitalWrite(motor_a_dir, FORWARD); analogWrite(motor_a_pwm, SPEED_0);
  digitalWrite(motor_b_dir, BACKWARD); analogWrite(motor_b_pwm, SPEED_80);
}

void back() // 후진
{
  digitalWrite(motor_a_dir, BACKWARD); analogWrite(motor_a_pwm, SPEED_80);
  digitalWrite(motor_b_dir, FORWARD); analogWrite(motor_b_pwm, SPEED_80);
}

void sstop() //정지
{
  digitalWrite(motor_a_dir, FORWARD); analogWrite(motor_a_pwm, SPEED_0);
  digitalWrite(motor_b_dir, FORWARD); analogWrite(motor_b_pwm, SPEED_0);
}