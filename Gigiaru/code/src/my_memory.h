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

    uint8_t entropy(const vector<uint8_t>& data){
        /*
        * 情報エントロピーの計算
        */

       return 4;
    }
    bool forgotten(const vector<uint8_t>& data){
        uint16_t ent=128*(uint16_t)data[ENTROPY];
        uint16_t time = (uint16_t)step%65536 - (256*(uint16_t)data[STAMP_M]+(uint16_t)data[STAMP_L]);
        return time>=ent;
    }
public:

    void add(vector<uint8_t> data){
        data.push_back(entropy(data));
        data.push_back(step%256);
        data.push_back((step/256)%256);
        xSemaphoreTake(xMemoryMutex, portMAX_DELAY);
        raw.push_back(data);
        xSemaphoreGive(xMemoryMutex);
        step++;
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