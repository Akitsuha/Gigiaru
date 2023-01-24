#include <Arduino.h>
#include <M5Atom.h>
#include <freertos/FreeRTOS.h>

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

#include <voice.h>
#include <motion.h>

#define PIN1 21  // 青
#define PIN2 25  // ピンク
#define PIN3 26  // 黄
#define PIN4 32  // オレンジ
#define STEP 2048
SStepper stepper1(STEP, PIN1, PIN3, PIN2, PIN4);  // オブジェクトを生成
TStepper s=TStepper(&stepper1,{{0,{RELATIVE,1024}}},2048);
motion m=motion({&s});

void demo_rot(SStepper* s);

void setup() {

  Serial.begin(115200);
  M5.begin();
  Blynk.begin(auth, ssid, pass);

  //ステッピングモーター

  //スピーカー
  voice_setup();

  //set_motion(&m);


  //タスク作成
  //xTaskCreateUniversal(motion_task,"motion_task",8192,NULL,1,NULL,APP_CPU_NUM);
  //xTaskCreateUniversal(voice_loop_task,"voice_loop_task",8192,NULL,1,NULL,APP_CPU_NUM);
}

void loop() {
  M5.update();
  Blynk.run();

  demo_rot(&stepper1);
  delay(100);
}



/**
 * <BlynkSimpleEsp32.h>でオブジェクト定義を行ってるので複数ファイルでincludeするとリンク時にエラー
 * 
*/
BLYNK_WRITE(V2)
{
  Serial.print("roar");
  //int angle = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  voice();
  //45degに1000ms角度固定
  //rotate(400);
}

BLYNK_WRITE(V5)
{
  int angle = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  //clock_set(6000);
  //45degに1000ms角度固定
}

void demo_rot(SStepper* s){
  SStepper* stepper=s;
  bool isMoving=true;
  unsigned long last_step_time=micros();
  int direction=1;
  int steps_left=600;
  int acc_step=60;          //加減速に要するstep数
  int acc=1;
  unsigned long step_delay=1000*acc_step/stepper->max_speed_spms/acc;
  
  while (isMoving)
  {
    unsigned long now=micros();
    Serial.printf("%lu/%lu\n",now - last_step_time,step_delay);
    if (now - last_step_time >= step_delay)
    {
      
      // get the timeStamp of when you stepped:
      last_step_time = now;
      // increment or decrement the step number,
      // depending on direction:
      if (direction == 1)
      {
          stepper->step_number++;
      }
      else
      {
          stepper->step_number--;
      }
      int step=stepper->step_number % 4;
      Serial.printf("step:%d\n",step);
      stepper->stepMotor(step);

      // 減速処理
      if (acc==steps_left or steps_left<acc_step)
      {
          step_delay=1000*acc_step/stepper->max_speed_spms/steps_left;
      }
      //加速処理
      else if (acc<acc_step)
      {
          acc++;
          step_delay=1000*acc_step/stepper->max_speed_spms/acc;
      }

      // decrement the steps left:
      steps_left--;

      if(steps_left==0)
      {
          stepper->sleep();
          isMoving=false;
      }
    }
  }
  
}