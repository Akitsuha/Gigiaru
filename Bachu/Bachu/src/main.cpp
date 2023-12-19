#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <freertos/FreeRTOS.h>
#include <math.h>
#include <stdlib.h>
#include <Wire.h>

//#define DEBUG

//#define MY_OTA

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

const char* ssidList[] = { "WARPSTAR-261622", "rs500k-b770d2-1", "iPhooone" };   // 接続を試みるSSIDのリスト
const char* passwordList[] = { "03CA88A54957B", "12a85eb203772", "s2jbe4om4rlj" };   // 各SSIDに対応するパスワードのリスト

#endif

#include "debug.h"
#include "myBlynk.h"
#include "component.h"
#include "bachu.h"

void setup() {

  Serial.begin(115200);
  #ifdef BLYNK
  myBlynk.begin(auth, ssidList, passwordList);
  #endif
  delay(1000);
  Serial.println("mainのsetupを開始");
  
  #ifdef MY_OTA
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
  #endif

  component_setup();
  task_setup();

}

void loop() {
  #ifdef MY_OTA
  ArduinoOTA.handle();
  #endif
  
  delay(1);
}


/**
 * <BlynkSimpleEsp32.h>でオブジェクト定義を行ってるので複数ファイルでincludeするとリンク時にエラー
 * 
*/
#ifdef BLYNK


#endif

