#ifndef Perception_h
#define  Perception_h

#include <vector>
#include "my_memory.h"
#include "gigi_component.h"
using std::vector;

#define STmemory_def 0
#define LPF_RATE 0.2 //1Byte値の更新なのであまり小さくするといつまでも更新されない
class Perception
{
private:
    GigiMemory* memory;
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

    u_int8_t hungriness=128;

public:
    Perception(GigiMemory* memory,std::map<int,ValActControler*> actuators):memory(memory),actuators(actuators){}
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

        //2-1.ToF
        for(int i=0;i<EYE_RESO*EYE_RESO;i++){
            u_int8_t dif=0;
            u_int8_t new_data=T_sig->get_raw()[i];
            u_int8_t STmemory_data=T_eye_STmemory[i];
            if(new_data>STmemory_data){
                dif=new_data-STmemory_data;
                T_eye_STmemory[i]+=dif*LPF_RATE;
            }
            else if(new_data<STmemory_data){
                dif=STmemory_data-new_data;
                T_eye_STmemory[i]-=dif*LPF_RATE;
            }
            #ifdef DEBUG
            //Serial.printf("new:%d,rec:%d,dif%d\n",new_data,STmemory_data,dif);
            #endif
            entr_sum+=dif;
        }
        for(int i=0;i<EYE_RESO*EYE_RESO;i++){
            u_int8_t dif=0;
            u_int8_t new_data=V_sig->get_raw()[i];
            u_int8_t STmemory_data=V_eye_STmemory[i];
            if(new_data>STmemory_data){
                dif=new_data-STmemory_data;
                V_eye_STmemory[i]+=dif*LPF_RATE;
            }
            else if(new_data<STmemory_data){
                dif=STmemory_data-new_data;
                V_eye_STmemory[i]-=dif*LPF_RATE;
            }
            entr_sum+=dif;
        }

        //2-2.hungry
        u_int8_t pre_hungriness=hungriness;
        hungriness=hungry.get_hungry();
        int16_t d_hungry=(int16_t)hungriness-(int16_t)pre_hungriness;

        u_int8_t entropy=entr_sum/(EYE_RESO*EYE_RESO*2);
        memory->add(new MemoryPiece{T_sig->get_avr(),V_sig->get_avr(),0,0,(u_int16_t)actuators[SERVO_ID]->get_value(),(int16_t)actuators[R_SERVO_ID]->get_value(),d_hungry,entropy,0});
    }

};

#endif