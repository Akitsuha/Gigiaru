#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <math.h>
#include <stdlib.h>

//#define Motion_Debug

//#define BLINK
#ifdef BLINK
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

#endif

#include "motion.h"
#include "Motion_servo.h"
#include "Motion_r_servo.h"
#include "Motion_library.h"
#include "voice.h"
#include "ToF.h"


void setup() {

  Serial.begin(115200);
  #ifdef BLYNK
  Blynk.begin(auth, ssid, pass);
  #endif

  //スピーカー
  //voice_setup();
  ToF_setup();
  InitI2SSpeakerOrMic(MODE_MIC);

  //タスク作成
  xTaskCreateUniversal(motion_task,"motion_task",8192,NULL,1,NULL,APP_CPU_NUM);

}

unsigned long pre_heatbeat=0;
int bpm=2000;
void loop() {
  #ifdef BLYNK
  Blynk.run();
  #endif
  
  hear();
  
  if(millis()-pre_heatbeat>bpm){
    //heart();
  }
  int n=rand()%20;
  if (n<=6)
  {
    revi_swing();
    //get_ToF();
  }
  else if(n<=7)
  {
    //joy();
  }
  
  delay(10);
}


/**
 * <BlynkSimpleEsp32.h>でオブジェクト定義を行ってるので複数ファイルでincludeするとリンク時にエラー
 * 
*/
#ifdef BLYNK
BLYNK_WRITE(V2)
{
  Serial.print("roar");

  voice("600");
}

BLYNK_WRITE(V5)
{
  float angle = param.asFloat();
  float now=servo.angle;
  TServo* s=new TServo(&servo,{{{500,angle}}});
  Motion* m=new Motion({s});
  
  set_motion(m);
}

BLYNK_WRITE(V1)
{
  float speed = param.asFloat(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  rservo.rotate(speed);
  //45degに1000ms角度固定
}

BLYNK_WRITE(V4)
{
  int motion = param.asInt(); // assigning incoming value from pin V1 to a variable
  
  start_motion(motion);
}

BLYNK_WRITE(V3)
{
  //Motion_R_Servo_h::stop(2);
  servo.release();
  //45degに1000ms角度固定
}
#endif

