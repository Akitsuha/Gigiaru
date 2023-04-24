#ifndef Decision_h
#define Decision_h

#include "my_memory.h"
#include "motion.h"

class Decision
{
private:
    GigiMemory* memory;
    ValActControler* servo_ctr;
    ValActControler* r_servo_ctr;
public:
    Decision(GigiMemory* memory,ValActControler* servo_ctr,ValActControler* r_servo_ctr):
            memory(memory),servo_ctr(servo_ctr),r_servo_ctr(r_servo_ctr){}
    void loop(){
        static u_int8_t trace_th=50;
        Memory_ptr last;

        xSemaphoreTake(xMemoryMutex, portMAX_DELAY);
        const vector<Memory_ptr>& memory_=memory->get();
        int num=memory_.size();
        if(num>0){
            last=memory_.back();
        }
        xSemaphoreGive(xMemoryMutex);
        //Serial.printf("memory size %d\n",num);
        if(num==0){
            return;
        }
        #ifdef DEBUG
        //Serial.printf("last ToFV:%d,ToFT:%d,Entropy:%d\n",last[TOF_V],last[TOF_T],last[ENTROPY]);
        #endif

        if(last->ToF_V-last->ToF_T>trace_th){
            turn(100,10);
        }
        else if(last->ToF_T-last->ToF_V>trace_th){
            turn(100,-10);
        }
    }
private:
    void turn(int duration,float angle){
        Motion_ptr motion=make_motion_ptr(new Motion(new EMG_rel(duration,angle,CSC_INTENTIONAL,100),nullptr));
        bool ret=motion->start(servo_ctr,r_servo_ctr);
        
        if(ret){
            /*2.メモリーに行った状況とモーションを記録*/

        }
    }
    void turn_to(){

    }
};



#endif