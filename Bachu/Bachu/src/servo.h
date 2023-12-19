#ifndef AK_Servo_h
#define AK_Servo_h

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "actuator.h"
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
bool isSetup = false;

class MyServo:public ValActuator
{
private:

public:
    MyServo(float angle_max, float default_angle, u_int8_t id=0):ValActuator(0.f,angle_max,default_angle,id){}
    virtual void update()=0;
    virtual void release()=0;
};

/*
class Servo_pwm:public Servo
{
private:
float setting[16][5];
int channel;
float duty_rate;
float duty_min;
float angle_max=0;

public:
Servo(int pin, int channel, float freq, float bit_num, float time_min, float time_max, float angle_max,float default_angle,u_int8_t id=0):ValActuator(0.f,angle_max,default_angle,id)
{
    Serial.println("------servo setup-------");
    this->channel=channel;
    this->angle_max=angle_max;
    setting[channel][0] = freq;       // 周波数
    setting[channel][1] = bit_num;    // bit数
    setting[channel][2] = time_min;   // 0゜パルス時間ms
    setting[channel][3] = time_max;   // 最大角度パルス時間ms
    setting[channel][4] = angle_max;  // 最大動作角度

    // サーボ定数算出
    float resolution = pow(2, setting[channel][1]); // 分解能（2のbit数乗）
    float period = 1000 / setting[channel][0];      // 周期（1000ms / 周波数） 
    duty_min = (resolution / period) * (setting[channel][2]); // 0゜Duty計算値（(分解能/周期)*0゜パルス時間μs）
    float duty_max = (resolution / period) * (setting[channel][3]); // 最大角度Duty計算値（(分解能/周期)*最大角度パルス時間μs）
    float duty_active = duty_max - duty_min;        // 動作範囲のDuty計算値
    duty_rate=(duty_active / setting[channel][4]);

    // PWM設定
    pinMode(pin, OUTPUT);               // PWM用ピンを出力設定
    ledcSetup(channel, freq, bit_num);  // PWM設定（ﾁｬﾝﾈﾙ, 周波数, ビット数）
    ledcAttachPin(pin, channel);        // PWMﾁｬﾝﾈﾙ割り当て（ピン番号, ﾁｬﾝﾈﾙ）

    update();
    release();
    Serial.println("------servo setup done-------");
}

void update(){
    int duty_value = (value * duty_rate) + duty_min;  // 角度指定値（浮動小数点数） → PWM出力Duty値（整数）換算

    if (duty_value <= duty_min) { // PWM出力Duty値が0゜Duty計算値以下なら
    duty_value = duty_min + 1;  // PWM出力Duty値＋1
    }
    // PWM出力実行
    
    ledcWrite(channel, duty_value); // 指定したﾁｬﾝﾈﾙにDuty値を指定してPWM出力実行
}

void release(){
    ledcWrite(channel, 0);
}
};*/

class Servo_drv:public MyServo
{
private:
int pin;
int time_min_us;
int time_max_us;
float angle_max=0;

public:
Servo_drv(int pin, float freq, int time_min_us, int time_max_us, float angle_max,float default_angle,u_int8_t id=0):MyServo(angle_max,default_angle,id)
{
    Serial.begin(115200);
    delay(1000);
    if(!isSetup){
        pwm.begin();     // 初期設定
        pwm.setPWMFreq(50);
        isSetup = true;
    }

    Serial.println("------servo setup-------");
    this->pin = pin;
    this->angle_max = angle_max;
    this->time_min_us = time_min_us;
    this->time_max_us = time_max_us;

    update();
    release();
    Serial.println("------servo setup done-------");
}

void update(){
    if (value > angle_max) { // PWM出力Duty値が0゜Duty計算値以下なら
        value = angle_max;  // PWM出力Duty値＋1
    }
    else if (value < 0)
    {
        value = 0;
    }
    pwm.writeMicroseconds(pin, map(value,0, angle_max, time_min_us, time_max_us)); 
}

void release(){
    pwm.writeMicroseconds(pin, 0); 
}
};


#endif
