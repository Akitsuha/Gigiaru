#ifndef Decision_h
#define Decision_h

#include "my_memory.h"
#include "planning.h"

class Decision
{
private:
    GigiMemory* memory;
    Planning* planning;
public:
    Decision(GigiMemory* memory,Planning* planning):memory(memory),planning(planning){}
    void loop(){
        static u_int8_t trace_th=50;
        vector<u_int8_t> last={};

        xSemaphoreTake(xMemoryMutex, portMAX_DELAY);
        const vector<vector<u_int8_t>>& memory_=memory->get();
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

        if(last[TOF_V]-last[TOF_T]>trace_th){
            planning->turn(100,10);
        }
        else if(last[TOF_T]-last[TOF_V]>trace_th){
            planning->turn(100,-10);
        }
    }
};



#endif