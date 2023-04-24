#ifndef MyMemory_h
#define MyMemory_h

#include <vector>
#include <memory>
#include <math.h>
using std::vector;
using std::unique_ptr;
using std::shared_ptr;


#define TOF_V 0
#define TOF_T 1
#define SOUND_LEVEL 2
#define HEAL 3
#define ENTROPY 4
#define STAMP_L 5
#define STAMP_M 6
#define NUM_DATA_TYPE 2

SemaphoreHandle_t xMemoryMutex = NULL;//I2C排他制御用のミューテックス

struct MemoryPiece{
    u_int8_t ToF_V;
    u_int8_t ToF_T;
    u_int8_t sound;
    u_int8_t food;
    u_int8_t entropy;
    u_int16_t stamp;
};

typedef shared_ptr<MemoryPiece> Memory_ptr;

class GigiMemory{
private:
    vector<Memory_ptr> raw={};
    vector<vector<u_int8_t>> experience={};
    //vector<u_int8_t> avr={};
    unsigned long step=0;
    int step_phase=0;
    
    bool forgotten(const Memory_ptr& data){
        uint16_t ent=128*(uint16_t)(data->entropy);
        uint16_t time = (uint16_t)step%65536 - data->stamp;
        return time>=ent;
    }

    Memory_ptr make_ptr(MemoryPiece* data){
        return shared_ptr<MemoryPiece>(data);
    }
public:
    GigiMemory(){
        xMemoryMutex = xSemaphoreCreateMutex();
        xSemaphoreGive(xMemoryMutex);
    }

    void add(MemoryPiece* data){
        data->stamp=step%65536;
        xSemaphoreTake(xMemoryMutex, portMAX_DELAY);
        raw.push_back(make_ptr(data));
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

    const vector<Memory_ptr>& get(){
        return raw;
    }
};



#endif