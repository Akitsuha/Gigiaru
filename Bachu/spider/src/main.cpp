#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <freertos/FreeRTOS.h>
#include <math.h>
#include <stdlib.h>
#include <Wire.h>

//#define DEBUG

//#define BLYNK
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

const char* ssidList[] = {"rs500k-b770d2-1", "WARPSTAR-261622",  "iPhooone" };   // 接続を試みるSSIDのリスト
const char* passwordList[] = {"12a85eb203772", "03CA88A54957B",  "s2jbe4om4rlj" };   // 各SSIDに対応するパスワードのリスト

#endif

#include "debug.h"
#include "myBlynk.h"
#include "component.h"
#include "bachu.h"

//Servo servo1;  // サーボオブジェクトを作成
void setup() {

  Serial.begin(115200);
  #ifdef BLYNK
  myBlynk.begin(auth, ssidList, passwordList);
  #endif
  delay(1000);
  Serial.println("mainのsetupを開始");
  
  
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

  //servo1.setPeriodHertz(50);      // Standard 50hz servo
  //servo1.attach(SERVO_PIN_11, 500, 2400);

  //task_setup();

}

void loop() {
  //voice_loop();
  ArduinoOTA.handle();
  static int pos = 0;      // position in degrees
  /*
  for (pos = 0; pos <= 180; pos += 1) { // sweep from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo1.write(pos);
    delay(2);             // waits 20ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // sweep from 180 degrees to 0 degrees
    servo1.write(pos);
    delay(2);
  }*/
  delay(1);
}


/**
 * <BlynkSimpleEsp32.h>でオブジェクト定義を行ってるので複数ファイルでincludeするとリンク時にエラー
 * 
*/
#ifdef BLYNK


#endif

