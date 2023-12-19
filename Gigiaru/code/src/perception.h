#ifndef Perception_h
#define  Perception_h

#include <vector>
#include "my_memory.h"
#include "gigi_component.h"
#include "myfunction.h"
using std::vector;

#define STmemory_def 0
#define LPF_RATE 0.2 //1Byte値の更新なのであまり小さくするといつまでも更新されない

class Perception{
private:
    GigiMemory* memory;
public:
    Perception(GigiMemory* memory):memory(memory){}
    virtual ~Perception(){};
    virtual void loop()=0;
};

class Perception1:Perception
{
private:
    
    //std::map<int,ToF_Eye*> sensors;
    std::map<int,ValActControler*> actuators;

    vector<u_int8_t> T_eye_STmemory={STmemory_def,STmemory_def,STmemory_def,STmemory_def,
                                    STmemory_def,STmemory_def,STmemory_def,STmemory_def,
                                    STmemory_def,STmemory_def,STmemory_def,STmemory_def,
                                    STmemory_def,STmemory_def,STmemory_def,STmemory_def};
    vector<u_int8_t> V_eye_STmemory={STmemory_def,STmemory_def,STmemory_def,STmemory_def,
                                    STmemory_def,STmemory_def,STmemory_def,STmemory_def,
                                    STmemory_def,STmemory_def,STmemory_def,STmemory_def,
                                    STmemory_def,STmemory_def,STmemory_def,STmemory_def};

    u_int8_t ToF_avr=0;
    u_int8_t entropy=0;

public:
    Perception1(GigiMemory* memory,std::map<int,ValActControler*> actuators):Perception(memory),actuators(actuators){}
    void loop(){
        /*
        * 1.Sensor,Actuatorクラスから知覚情報を取得
        */
       #ifdef DEBUG
       #endif
       
        shared_ptr<Signal> T_sig=T_eye->get();
        //shared_ptr<Signal> T_sig=sensors[T_ToF_ID]->get();
        shared_ptr<Signal> V_sig=V_eye->get();
        
        /*
        * 2.情報を処理し、WorkMemoryを更新
        */
        u_int16_t entr_sum=0;//0~255*EYE_RESO^2*2
        u_int16_t ToF_sum=0;//0~255*EYE_RESO^2*2

        /*
        * 2-1.ToF
        * 
        */
        for(int i=0;i<EYE_RESO*EYE_RESO;i++){
            entr_sum+=unsigned_LPF(T_eye_STmemory[i],T_sig->get_raw()[i],LPF_RATE);
            ToF_sum+=T_sig->get_raw()[i];
        }
        for(int i=0;i<EYE_RESO*EYE_RESO;i++){
            entr_sum+=unsigned_LPF(V_eye_STmemory[i],V_sig->get_raw()[i],LPF_RATE);
            ToF_sum+=V_sig->get_raw()[i];
        }

        entropy=entr_sum/(EYE_RESO*EYE_RESO*2);
        ToF_avr=ToF_sum/(EYE_RESO*EYE_RESO*2);
        //memory->add(new MemoryPiece{T_sig->get_avr(),V_sig->get_avr(),0,0,(u_int16_t)actuators[SERVO_ID]->get_value(),(int16_t)actuators[R_SERVO_ID]->get_value(),d_hungry,entropy,0});
    }

    u_int8_t get_ToF_avr(){
        return ToF_avr;
    }

    u_int8_t get_entropy(){
        return entropy;
    }
};

class Perception2:Perception{
private:
    bool approve=false;

public:
    Perception2(GigiMemory* memory):Perception(memory){}
    void loop(){
        //
    }
};

#endif