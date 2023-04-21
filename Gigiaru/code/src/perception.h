#ifndef Perception_h
#define  Perception_h

#include "sight.h"
#include "my_memory.h"

class Perception
{
private:
    GigiMemory* memory;
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
        vector<u_int8_t> data={T_sig->get_avr(),V_sig->get_avr(),0,0};
        memory->add(data);
    }
};

#endif