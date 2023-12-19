#ifndef Component_h
#define Component_h

#include "voice.h"
#include "sight.h"
#include "servo.h"
#include "r_servo.h"
#include "my_memory.h"
#include "phygiology.h"

GigiMemory memory;
void memory_task(void *pvParameters){
    while (1)
    {
        //memory.oblivion();
        delay(100);
    }
}

#define EYE_RESO 4
#define T_ToF_ID 1
#define V_ToF_ID 2
TMF8821_Eye* T_eye;
VL53L5CX_Eye* V_eye;
void ToF_setup(){
    xMutex = xSemaphoreCreateMutex();
    xSemaphoreGive(xMutex);
    T_eye=new TMF8821_Eye(T_ToF_ID,EYE_RESO);
    V_eye=new VL53L5CX_Eye(V_ToF_ID,EYE_RESO);
    
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

std::map<int,ToF_Eye*> sensors = {{T_ToF_ID,T_eye},{V_ToF_ID,V_eye}};



#define SERVO_PIN 25
#define SERVO_ID 1
#define SERVO_ANGLE_RANGE 160
AK::Servo servo_=AK::Servo(SERVO_PIN,0,50,12,0.5,2.4,SERVO_ANGLE_RANGE,80,SERVO_ID);
ValActControler servo_ctr=ValActControler(servo_);
void servo_task(void *pvParameters){
    while (1)
    {
        servo_ctr.loop();
        //delayMicroseconds(100);
        delay(1);
    }
}

#define R_SERVO_PIN 21
#define R_SERVO_ID 2
AK::R_Servo r_servo_=AK::R_Servo(R_SERVO_PIN,2,50,16,4885,65,350,R_SERVO_ID);
ValActControler r_servo_ctr=ValActControler(r_servo_);
void r_servo_task(void *pvParameters){
    while (1)
    {
        r_servo_ctr.loop();
        //delayMicroseconds(100);
        delay(1);
    }
    
}

std::map<int,ValActControler*> actuators={{SERVO_ID,&servo_ctr},{R_SERVO_ID,&r_servo_ctr}};

Hungry hungry;


#endif