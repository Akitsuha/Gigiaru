#ifndef Component_h
#define Component_h

#include "servo.h"
#include "act_controller.h"

std::map<int,ValActControler*> actuators={};

#define SERVO_PIN_11 0
#define SERVO_PIN_12 1
#define SERVO_PIN_21 2
#define SERVO_PIN_22 3
#define SERVO_PIN_31 4
#define SERVO_PIN_32 5
#define SERVO_PIN_41 6
#define SERVO_PIN_42 7
#define SERVO_ID_11 0
#define SERVO_ID_12 1
#define SERVO_ID_21 2
#define SERVO_ID_22 3
#define SERVO_ID_31 4
#define SERVO_ID_32 5
#define SERVO_ID_41 6
#define SERVO_ID_42 7
#define SERVO_ANGLE_RANGE 180
ValActControler* servo11;
void servo_task(void *pvParameters){
    
    while (1)
    {
        //servo11.loop();
        //delayMicroseconds(100);
        delay(1);
    }
}

void component_setup(){
    actuators[SERVO_ID_11] = new ValActControler(new Servo_drv(SERVO_PIN_11,50,500,2400,SERVO_ANGLE_RANGE,90,SERVO_ID_11));
}




#endif