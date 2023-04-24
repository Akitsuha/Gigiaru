#ifndef MyMemory_h
#define MyMemory_h

#include "work_memory.h"
#include <math.h>

#define TOF_V 0
#define TOF_T 1
#define SOUND_LEVEL 2
#define HEAL 3
#define ENTROPY 4
#define STAMP_L 5
#define STAMP_M 6
#define NUM_DATA_TYPE 2

class GigiMemory:public Memory{
private:

    vector<u_int8_t> avr={};
    unsigned long step=0;
    int step_phase=0;
    
    bool forgotten(const vector<uint8_t>& data){
        uint16_t ent=128*(uint16_t)data[ENTROPY];
        uint16_t time = (uint16_t)step%65536 - (256*(uint16_t)data[STAMP_M]+(uint16_t)data[STAMP_L]);
        return time>=ent;
    }
public:

    void add(vector<uint8_t> data){
        data.push_back(step%256);
        data.push_back((step/256)%256);
        xSemaphoreTake(xMemoryMutex, portMAX_DELAY);
        raw.push_back(data);
        xSemaphoreGive(xMemoryMutex);
        step++;

        /*3.価値の高いイベントが起こった時、直前に行ったモーション(add_actionで記録)とともに記録*/
    }

    void add_action(/*状況,モーション*/){
        /*1.直前にあったエントロピーの高い記憶をピック*/

        /*2.イベント、状況、モーションを記録*/
    }

    void oblivion(){
        auto itr=raw.begin();
        xSemaphoreTake(xMemoryMutex, portMAX_DELAY);
        while (itr!=raw.end())
        {
            if(forgotten(*itr)){
                itr=raw.erase(itr);
            }
            else{
                itr++;
            }
        }
        xSemaphoreGive(xMemoryMutex);
        
    }
};



#endif