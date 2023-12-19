#ifndef MyMemory_h
#define MyMemory_h

#include <vector>
#include <memory>
#include <math.h>
#include "motion.h"
using std::vector;
using std::unique_ptr;
using std::shared_ptr;

SemaphoreHandle_t xMemoryMutex = NULL;//I2C排他制御用のミューテックス

struct MemoryPiece{
    u_int8_t ToF_V;
    u_int8_t ToF_T;
    u_int8_t sound;
    u_int8_t food;
    u_int16_t servo;
    int16_t r_servo;
    int16_t d_hungry;
    u_int8_t entropy;
    u_int16_t stamp;
};
typedef shared_ptr<MemoryPiece> Memory_ptr;

struct ActionValue{
    Memory_ptr situation;
    Motion_ptr motion;
    u_int8_t value;
};
typedef shared_ptr<ActionValue> ActionVal_ptr;
ActionVal_ptr make_actionval_ptr(ActionValue* av){
    return shared_ptr<ActionValue>(av);
}

#define ToF_DIST 160

class GigiMemory{
private:
    //vector<Memory_ptr> raw={};

    u_int8_t ToF_dist[ToF_DIST]={};

    //ActionVal_ptr most_valuable=nullptr;
    //Motion_ptr last_motion=nullptr;

    unsigned long step=0;
    int step_phase=0;
    
    /*
    bool forgotten(const Memory_ptr& data){
        uint16_t ent=128*(uint16_t)(data->entropy);
        uint16_t time = (uint16_t)step%65536 - data->stamp;
        return time>=ent;
    }*/

    void ToF_dist_update(float center,u_int8_t value){
        static float norm_factor = -0.5/pow(5,2);
        float center_block = center*ToF_DIST/160;
        for(int i=0;i<ToF_DIST;i++){
            float update_rate=pow(M_E,(center_block-i)*(center_block-i)*norm_factor);
            float ex_value=(float)(ToF_dist[i]);
            if(value>ex_value){
                ToF_dist[i]+= (u_int8_t)((value-ex_value)*update_rate);
            }
            else if(value<ex_value){
                ToF_dist[i]-= (u_int8_t)((ex_value-value)*update_rate);
            }
            
        }
    }

    Memory_ptr make_ptr(MemoryPiece* data){
        return shared_ptr<MemoryPiece>(data);
    }
public:
    GigiMemory(){
        xMemoryMutex = xSemaphoreCreateMutex();
        xSemaphoreGive(xMemoryMutex);
    }

    u_int8_t value_fx(Memory_ptr memory){
        if(memory->d_hungry>10){
            return 200;
        }
        else{
            return (memory->entropy)/2+(memory->ToF_T)/4+(memory->ToF_V)/4;
        }
        
    }

    void add(MemoryPiece* data){
        
        data->stamp=step%65536;
        Memory_ptr this_memory=make_ptr(data);
        u_int8_t value=value_fx(this_memory);
        /*
        xSemaphoreTake(xMemoryMutex, portMAX_DELAY);
        raw.push_back(this_memory);
        xSemaphoreGive(xMemoryMutex);*/

        ToF_dist_update(data->servo,(data->ToF_T)/2+(data->ToF_V)/2);

        step++;

        /*3.価値の高いイベントが起こった時、直前に行ったモーション(add_actionで記録)とともに記録
        if(most_valuable==nullptr){
            if(last_motion!=nullptr){
                most_valuable=make_actionval_ptr(new ActionValue({this_memory,last_motion,value}));
            }
        }
        else{
            if(most_valuable->value <= value){
                most_valuable=make_actionval_ptr(new ActionValue({this_memory,last_motion,value}));
            }
        }*/
    }

    /*
    void add_action(Motion_ptr motion){
        last_motion=motion;
    }

    ActionVal_ptr get_valuable_action(){
        return most_valuable;
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
    }*/

    u_int8_t get_ToFdist(int cell_id){
        if(cell_id<0 || cell_id>ToF_DIST){
            return 0;
        }
        return ToF_dist[cell_id];
    }

    u_int8_t get_ToFdistMax(int cell_id_from,int cell_id_to){
        if(cell_id_from>=cell_id_to || cell_id_from>=ToF_DIST || cell_id_to<=0){
            return 0;
        }
        if(cell_id_from<0){
            cell_id_from=0;
        }
        if(cell_id_to>ToF_DIST){
            cell_id_to=ToF_DIST;
        }
        return *std::max_element(ToF_dist+cell_id_from,ToF_dist+cell_id_to);
    }

    float get_valuest_cell(){
        return std::distance(ToF_dist, std::max_element(ToF_dist, ToF_dist + ToF_DIST))*160./ToF_DIST;
    }
};



#endif