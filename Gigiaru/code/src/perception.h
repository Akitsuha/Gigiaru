#ifndef Perception_h
#define  Perception_h

#include <vector>
#include "sight.h"
#include "my_memory.h"
using std::vector;

#define STmemory_def 0
#define LPF_RATE 0.2 //1Byte値の更新なのであまり小さくするといつまでも更新されない
class Perception
{
private:
    GigiMemory* memory;
    vector<u_int8_t> T_eye_STmemory={STmemory_def,STmemory_def,STmemory_def,STmemory_def,
                                    STmemory_def,STmemory_def,STmemory_def,STmemory_def,
                                    STmemory_def,STmemory_def,STmemory_def,STmemory_def,
                                    STmemory_def,STmemory_def,STmemory_def,STmemory_def};
    vector<u_int8_t> V_eye_STmemory={STmemory_def,STmemory_def,STmemory_def,STmemory_def,
                                    STmemory_def,STmemory_def,STmemory_def,STmemory_def,
                                    STmemory_def,STmemory_def,STmemory_def,STmemory_def,
                                    STmemory_def,STmemory_def,STmemory_def,STmemory_def};

public:
    Perception(GigiMemory* memory):memory(memory){}
    void loop(){
        /*
        * 1.Sensorクラスから知覚情報を取得
        */
        shared_ptr<Signal> T_sig=T_eye->get();
        shared_ptr<Signal> V_sig=V_eye->get();
        
        /*
        * 2.情報を処理し、WorkMemoryを更新
        */
        u_int16_t entr_sum=0;//0~255*EYE_RESO^2*2
        for(int i=0;i++;i<EYE_RESO*EYE_RESO){
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
            entr_sum+=dif;
        }
        for(int i=0;i++;i<EYE_RESO*EYE_RESO){
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
        u_int8_t entropy=entr_sum/(EYE_RESO*EYE_RESO*2);
        vector<u_int8_t> data={T_sig->get_avr(),V_sig->get_avr(),0,0,entropy};
        memory->add(data);
    }
};

#endif