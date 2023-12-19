#ifndef Bachu_h
#define Bachu_h

#include <algorithm>
#include "component.h"
#include "decision.h"

#include <BlynkSimpleEsp32.h>

Decision1 decision1(actuators);
void decision1_task(void *pvParameters){
    while (1)
    {
        decision1.loop();
        delay(1);
    }
}

#ifdef BLYNK
#define BLYNK_LCD V4
WidgetLCD lcd(BLYNK_LCD);


void blynk_task(void *pvParameters){
    while (1)
    {   
        Blynk.run();
        static unsigned long pre_update=millis();
        static bool led=1;
        if(millis()-pre_update>1000){
            Blynk.virtualWrite(V6,led);
            led=!led;
            pre_update=millis();
        }
        
        delay(10);
    }
}
#endif

TaskHandle_t servo_handle;
TaskHandle_t decision_handle;

void task_setup(){
    
    xTaskCreateUniversal(servo_task,"servo_task",4096,NULL,1,&servo_handle,APP_CPU_NUM);
    xTaskCreateUniversal(decision1_task,"decision1_task",4096,NULL,1,&decision_handle,APP_CPU_NUM);
    #ifdef BLYNK
    xTaskCreateUniversal(blynk_task,"blynk_task",4096,NULL,1,NULL,PRO_CPU_NUM);
    Blynk.virtualWrite(10,0);
    #endif
}

#endif