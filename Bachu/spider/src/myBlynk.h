#ifndef BLYNK_ADM_h
#define BLYNK_ADM_h

#include <BlynkSimpleEsp32.h>

/*
 * Blynkクラスを使いやすいようにラッパー
 */
class MyBlynk:public BlynkWifi{

public:
    using BlynkWifi::BlynkWifi;//継承コンストラクタ

    //wifiに接続しなければpass
    bool begin(const char* auth,
               const char* ssid,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT,
               u_int32_t timeout=18000U)
    {
        connectWiFi(ssid, pass);
        config(auth, domain, port);
        return this->connect(timeout);
    }

    bool begin(const char* auth,const char* ssidList[],const char* passwordList[]){
        int numNetworks = sizeof(ssidList) / sizeof(ssidList[0]);   // SSIDのリストの要素数

        for (int i = 0; i < numNetworks; ++i) {
            Serial.print("Connecting to ");
            Serial.println(ssidList[i]);

            if (begin(auth,ssidList[i], passwordList[i])) {
                Serial.println("");
                Serial.print("Connected to ");
                Serial.println(ssidList[i]);
                return true;
            } else {
                Serial.println("");
                Serial.println("Connection failed");
            }
        }
        return false;
    }

};

MyBlynk myBlynk(_blynkTransport);

WidgetTerminal terminal(V8);

class BlynkAdmin
{

public:
    

};

#endif