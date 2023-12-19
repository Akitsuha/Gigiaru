#ifndef Component_h
#define Component_h

#include "servo.h"
#include "act_controller.h"
//#include <ESP32Servo.h>

#define SERVO_PIN_11 32
#define SERVO_PIN_12 33
#define SERVO_ID_11 1
#define SERVO_ID_12 2
/*
#define SERVO_PIN_21 25
#define SERVO_PIN_22 25
#define SERVO_PIN_31 25
#define SERVO_PIN_32 25
#define SERVO_PIN_41 25
#define SERVO_PIN_42 25

#define SERVO_ID_21 3
#define SERVO_ID_22 4
#define SERVO_ID_31 5
#define SERVO_ID_32 6
#define SERVO_ID_41 7
#define SERVO_ID_42 8*/
#define SERVO_ANGLE_RANGE 180
//ValActControler servo11=ValActControler(new AK::Servo(SERVO_PIN_11,0,50,12,0.5,2.4,SERVO_ANGLE_RANGE,80,SERVO_ID_11));
//ValActControler servo12=ValActControler(new AK::Servo(SERVO_PIN_12,1,50,12,0.5,2.4,SERVO_ANGLE_RANGE,80,SERVO_ID_12));
/*ValActControler servo21=ValActControler(new AK::Servo(SERVO_PIN_21,0,50,12,0.5,2.4,SERVO_ANGLE_RANGE,80,SERVO_ID_21));
ValActControler servo22=ValActControler(new AK::Servo(SERVO_PIN_22,0,50,12,0.5,2.4,SERVO_ANGLE_RANGE,80,SERVO_ID_22));
ValActControler servo31=ValActControler(new AK::Servo(SERVO_PIN_31,0,50,12,0.5,2.4,SERVO_ANGLE_RANGE,80,SERVO_ID_31));
ValActControler servo32=ValActControler(new AK::Servo(SERVO_PIN_32,0,50,12,0.5,2.4,SERVO_ANGLE_RANGE,80,SERVO_ID_32));
ValActControler servo41=ValActControler(new AK::Servo(SERVO_PIN_41,0,50,12,0.5,2.4,SERVO_ANGLE_RANGE,80,SERVO_ID_41));
ValActControler servo42=ValActControler(new AK::Servo(SERVO_PIN_42,0,50,12,0.5,2.4,SERVO_ANGLE_RANGE,80,SERVO_ID_42));*/

void servo_task(void *pvParameters){
    while (1)
    {
        //servo11.loop();
        //servo12.loop();
        /*servo21.loop();
        servo22.loop();
        servo31.loop();
        servo32.loop();
        servo41.loop();
        servo42.loop();*/
        //delayMicroseconds(100);
        //Serial.println("servo loop");
        delay(1);
    }
}

std::map<int,ValActControler*> actuators={
    //{SERVO_ID_11,&servo11},{SERVO_ID_12,&servo12}
    };


#endif