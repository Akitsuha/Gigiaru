#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <math.h>
#include <stdlib.h>
#include <Wire.h>

#define DEBUG


#define BLYNK
#ifdef BLYNK
// Template ID, Device Name and Auth Token are provided by the Blynk.Cloud
// See the Device Info tab, or Template settings
#define BLYNK_TEMPLATE_ID           "TMPL_AIhQGLk"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "oKokEZz4srgRbbC1OV4L_q1YtuffKOW2"
// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

//#include <ESP32Servo.h>
char auth[] = BLYNK_AUTH_TOKEN;
// Your WiFi credentials.
// Set password to "" for open networks.
const char* ssid = "rs500k-b770d2-1";
const char* pass = "12a85eb203772";
//const char* ssid = "iPhooone";
//const char* pass = "s2jbe4om4rlj";

#endif

#include "gigi.h"

void setup() {

  Serial.begin(115200);
  #ifdef BLYNK
  Blynk.begin(auth, ssid, pass);
  #endif
  delay(1000);
  Serial.println("mainのsetupを開始");

  //スピーカー
  //voice_setup();
  
  //InitI2SSpeakerOrMic(MODE_MIC);

  Wire1.begin(26,32); //This resets to 100kHz I2C
  Wire1.setClock(400000); //Sensor has max I2C freq of 400kHz 

  ToF_setup();

  //タスク作成
  //xTaskCreateUniversal(motion_task,"motion_task",8192,NULL,1,NULL,APP_CPU_NUM);
  task_setup();
}

unsigned long pre_heatbeat=0;
int bpm=2000;
void loop() {
  #ifdef BLYNK
  
  #endif
  
  //hear();
  /*
  if(millis()-pre_heatbeat>bpm){
    heart();
  }
  int n=rand()%20;
  if (n<=6)
  {
    //revi_swing();
    //get_ToF();
  }
  else if(n<=7)
  {
    //joy();
  }*/
  
  delay(10);
}


/**
 * <BlynkSimpleEsp32.h>でオブジェクト定義を行ってるので複数ファイルでincludeするとリンク時にエラー
 * 
*/
#ifdef BLYNK
BLYNK_WRITE(V5)
{
  float angle = param.asFloat();
  Serial.printf("blynk set V5:%f\n",angle);
  servo_ctr.add(new EMG_abs(100,angle,6,190));
}

BLYNK_WRITE(V1)
{
  float speed = param.asFloat(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  r_servo_ctr.add(new EMG_abs(1000,speed,6,251));
  //45degに1000ms角度固定
}

#endif

