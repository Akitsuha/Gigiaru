#ifndef Gigi_h
#define Gigi_h

#include <algorithm>
#include "gigi_component.h"
#include "decision.h"
#include "perception.h"
#include "biokinesis.h"

#include <BlynkSimpleEsp32.h>

Decision decision(&memory,actuators);
void decision_task(void *pvParameters){
    while (1)
    {
        decision.loop();
        delay(10);
    }
}

Perception perception(&memory,actuators);
void perception_task(void *pvParameters){
    while (1)
    {
        perception.loop();
        delay(1);
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
#define BLYNK_LCD V4
WidgetLCD lcd(BLYNK_LCD);

void Blynk_ToF_image(){

    shared_ptr<Signal> T_sig=T_eye->get();
    shared_ptr<Signal> V_sig=V_eye->get();
    vector<u_int8_t> T_image=T_sig->get_raw();
    vector<u_int8_t> V_image=V_sig->get_raw();
    if(T_image.size()==EYE_RESO*EYE_RESO && V_image.size()==EYE_RESO*EYE_RESO){
        for (int y = 0 ; y < EYE_RESO ; y ++)
        {
            for (int x = 0 ; x< EYE_RESO;  x++)
            {
                Blynk.virtualWrite(x+y*EYE_RESO+16,T_image[x+y*EYE_RESO]);
            }
        }
    }

}

void Blynk_ToF_distribute(){
    for (int y = 0 ; y < 16 ; y ++)
    {
        Blynk.virtualWrite(y+32,memory.get_ToFdistMax(y*10,(y+1)*10));
    }
}
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
            Blynk_ToF_image();
            Blynk_ToF_distribute();
            pre_update=millis();
        }
        
        delay(10);
    }
}
#endif

TaskHandle_t sight_handle;
TaskHandle_t servo_handle;
TaskHandle_t r_servo_handle;
TaskHandle_t memory_handle;
TaskHandle_t perception_handle;
TaskHandle_t decision_handle;
TaskHandle_t biokinesis_handle;

void task_setup(){
    
    xTaskCreateUniversal(sight_task,"sight_task",4096,NULL,1,&sight_handle,APP_CPU_NUM);
    xTaskCreateUniversal(servo_task,"servo_task",4096,NULL,1,&servo_handle,APP_CPU_NUM);
    xTaskCreateUniversal(r_servo_task,"r_servo_task",4096,NULL,1,&r_servo_handle,APP_CPU_NUM);
    xTaskCreateUniversal(memory_task,"memory_task",4096,NULL,1,&memory_handle,APP_CPU_NUM);
    xTaskCreateUniversal(perception_task,"perception_task",4096,NULL,1,&perception_handle,APP_CPU_NUM);
    xTaskCreateUniversal(decision_task,"decision_task",4096,NULL,1,&decision_handle,APP_CPU_NUM);
    xTaskCreateUniversal(biokinesis_task,"biokinesis_task",4096,NULL,1,&biokinesis_handle,APP_CPU_NUM);
    #ifdef BLYNK
    xTaskCreateUniversal(blynk_task,"blynk_task",4096,NULL,1,NULL,PRO_CPU_NUM);
    Blynk.virtualWrite(10,0);
    #endif
}

bool _deep_sleep=false;

void deep_sleep(){
    if(!_deep_sleep){
        vTaskSuspend(sight_handle);
        vTaskSuspend(servo_handle);
        vTaskSuspend(r_servo_handle);
        vTaskSuspend(memory_handle);
        vTaskSuspend(perception_handle);
        vTaskSuspend(decision_handle);
        vTaskSuspend(biokinesis_handle);
        _deep_sleep=true;
    }
    
}

void wake_up(){
    if(_deep_sleep){
        vTaskResume(sight_handle);
        vTaskResume(servo_handle);
        vTaskResume(r_servo_handle);
        vTaskResume(memory_handle);
        vTaskResume(perception_handle);
        vTaskResume(decision_handle);
        vTaskResume(biokinesis_handle);
        _deep_sleep=false;
    }
    
}

#endif