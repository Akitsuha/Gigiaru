#ifndef Gigi_h
#define Gigi_h

#include "voice.h"
#include "sight.h"
#include "servo.h"
#include "r_servo.h"
#include "memory.h"
#include "perception.h"
#include "decision.h"
#include "biokinesis.h"

#include <BlynkSimpleEsp32.h>

GigiMemory memory;
void memory_task(void *pvParameters){
    while (1)
    {
        memory.oblivion();
        delay(100);
    }
}

Perception perception(&memory);
void perception_task(void *pvParameters){
    while (1)
    {
        perception.loop();
        delay(1);
    }
}

void sight_task(void *pvParameters){
    while (1)
    {
        T_eye->update();
        V_eye->update();
        //ToF_image();
        delay(1);
    }
}

#define SERVO_PIN 25
AK::Servo servo_=AK::Servo(SERVO_PIN,0,50,12,0.5,2.4,160,80,1);
ValActControler servo_ctr=ValActControler(servo_);
void servo_task(void *pvParameters){
    while (1)
    {
        servo_ctr.loop();
        //delayMicroseconds(100);
        delay(10);
    }
}

#define R_SERVO_PIN 21
AK::R_Servo r_servo_=AK::R_Servo(R_SERVO_PIN,2,50,16,4885,65,350,2);
ValActControler r_servo_ctr=ValActControler(r_servo_);
void r_servo_task(void *pvParameters){
    while (1)
    {
        r_servo_ctr.loop();
        //delayMicroseconds(100);
        delay(10);
    }
    
}

Planning planning(&servo_ctr);

Decision decision(&memory,&planning);
void decision_task(void *pvParameters){
    while (1)
    {
        decision.loop();
        delay(10);
    }
}

Biokinesis biokinesis(&servo_ctr,&r_servo_ctr);
void biokinesis_task(void *pvParameters){
    while (1)
    {
        biokinesis.loop();
        delay(10);
    }
}

#ifdef BLYNK
void blynk_task(void *pvParameters){
    while (1)
    {   
        Blynk.run();
        static unsigned long pre_update=millis();
        static bool led=1;
        if(millis()-pre_update>1000){
            Blynk.virtualWrite(V6,led);
            led=!led;
            shared_ptr<Signal> T_sig=T_eye->get();
            shared_ptr<Signal> V_sig=V_eye->get();
            Blynk.virtualWrite(5,servo_.get_value());
            Blynk.virtualWrite(0,T_sig->get_avr());
            Blynk.virtualWrite(7,V_sig->get_avr());
            pre_update=millis();
        }
        
        delay(10);
    }
}
#endif

void task_setup(){
    xTaskCreateUniversal(sight_task,"sight_task",4096,NULL,1,NULL,APP_CPU_NUM);
    xTaskCreateUniversal(servo_task,"servo_task",4096,NULL,1,NULL,APP_CPU_NUM);
    xTaskCreateUniversal(r_servo_task,"r_servo_task",4096,NULL,1,NULL,APP_CPU_NUM);
    xTaskCreateUniversal(memory_task,"memory_task",4096,NULL,1,NULL,APP_CPU_NUM);
    xTaskCreateUniversal(perception_task,"perception_task",4096,NULL,1,NULL,APP_CPU_NUM);
    xTaskCreateUniversal(decision_task,"decision_task",4096,NULL,1,NULL,APP_CPU_NUM);
    xTaskCreateUniversal(biokinesis_task,"biokinesis_task",4096,NULL,1,NULL,APP_CPU_NUM);
    #ifdef BLYNK
    xTaskCreateUniversal(blynk_task,"blynk_task",4096,NULL,1,NULL,PRO_CPU_NUM);
    #endif
}

#endif