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
        while(!Blynk.connected()) {
            Blynk.begin(auth, ssid, pass);
            delay(1000);
        }
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
            Blynk.virtualWrite(11,perception1.get_ToF_avr());
            Blynk_ToF_image();
            Blynk_ToF_distribute();
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