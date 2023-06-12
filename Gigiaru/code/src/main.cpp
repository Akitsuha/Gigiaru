#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <freertos/FreeRTOS.h>
#include <math.h>
#include <stdlib.h>
#include <Wire.h>

//#define DEBUG

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

char auth[] = BLYNK_AUTH_TOKEN;
// Your WiFi credentials.
// Set password to "" for open networks.
//const char* ssid = "WARPSTAR-261622";
//const char* pass = "03CA88A54957B";
const char* ssid = "rs500k-b770d2-1";
const char* pass = "12a85eb203772";
//const char* ssid = "iPhooone";
//const char* pass = "s2jbe4om4rlj";

#endif

#include "gigi.h"
#include "Motion_library.h"

void setup() {

  Serial.begin(115200);
  #ifdef BLYNK
  Blynk.begin(auth, ssid, pass);
  #endif
  delay(1000);
  Serial.println("mainのsetupを開始");


  //ArduinoOTA.setHostname("myesp32");
  
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

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

void loop() {
  //voice_loop();
  ArduinoOTA.handle();
  delay(1);
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

/*
BLYNK_WRITE(V2)
{
  voice(FILENAME);
}*/

BLYNK_WRITE(V3)
{
  int motion_id = param.asInt(); // assigning incoming value from pin V1 to a variable

  Motion_ptr motion=make_motion_ptr(new Motion(dance));
  Serial.printf("blynk set V3:%d\n",motion_id);
  motion->start(actuators);
  //45degに1000ms角度固定
}

BLYNK_WRITE(V10)
{
  int sleep = param.asInt(); // assigning incoming value from pin V1 to a variable

  if(sleep==0){
    wake_up();
  }
  else if(sleep==1){
    deep_sleep();
  }
}

#endif

