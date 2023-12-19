
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_PWMServoDriver.h>



// Blynkアプリで生成されるAuth Tokenを設定してください
#define BLYNK_TEMPLATE_ID           "TMPL_AIhQGLk"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "oKokEZz4srgRbbC1OV4L_q1YtuffKOW2"
char auth[] = BLYNK_AUTH_TOKEN;

// Wi-Fi情報を設定してください
char ssid[] = "rs500k-b770d2-1";
char pass[] = "12a85eb203772";

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

// Blynkの仮想ピンとサーボモータの接続
#define SERVO_PIN V5

void setup()
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  pwm.begin();     // 初期設定
  pwm.setPWMFreq(50);

}

void loop()
{
  Blynk.run();
}

// Blynkアプリのスライダーから角度を受け取り、サーボモータを制御するコールバック関数
BLYNK_WRITE(SERVO_PIN)
{
  int angle = param.asInt();
  pwm.writeMicroseconds(0, map(angle,0, 180, 500, 2400));
}
