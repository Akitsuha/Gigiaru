#ifndef AK_RServo_h
#define AK_RServo_h

#include "actuator.h"

#define R_SERVO_MAX 128.f

namespace AK{
    class R_Servo:public ValActuator
    {
    private:
    float setting[16][5];
    int channel;
    float center_pwm;
    float deadband;
    float validband;

    
    public:
    R_Servo(int pin, int channel, float freq, float bit_num, float center_pwm,float deadband,float max_band,u_int8_t id=0)
        :ValActuator(-1.f*R_SERVO_MAX,R_SERVO_MAX,0.f,id),channel(channel),center_pwm(center_pwm),deadband(deadband),validband(max_band-deadband)
    {
        setting[channel][0] = freq;       // 周波数
        setting[channel][1] = bit_num;    // bit数


        // PWM設定
        pinMode(pin, OUTPUT);               // PWM用ピンを出力設定
        ledcSetup(channel, freq, bit_num);  // PWM設定（ﾁｬﾝﾈﾙ, 周波数, ビット数）
        ledcAttachPin(pin, channel);        // PWMﾁｬﾝﾈﾙ割り当て（ピン番号, ﾁｬﾝﾈﾙ）

        update();
        release();
    }

    void update(){

        int duty_value = center_pwm;
        if(value>0){
            duty_value += deadband + validband * (value/R_SERVO_MAX);
        }
        else if (value<0)
        {
            duty_value -= deadband + validband * (-value/R_SERVO_MAX);
        }
        
        ledcWrite(channel, duty_value); // 指定したﾁｬﾝﾈﾙにDuty値を指定してPWM出力実行
    }

    void release(){
        ledcWrite(channel, 0);
        #ifdef DEBUG
        //Serial.println("rservo release");
        #endif
    }
    };
}

#endif
