#ifndef ActControler_h
#define ActControler_h

#include <Tween.h>
#include <vector>
#include "actuator.h"
#include "emg.h"
#include <memory>

using std::vector;
using std::unique_ptr;

class ActControler
{
protected:
    u_int8_t id=0;
public:
    virtual ~ActControler(){}//仮想デストラクタ
    virtual void loop()=0;
};

enum VAct_op{
    BACK,
    SHIFT,
};

#define CSC_INTENTIONAL 5
#define CSC_PHYSIC 3
#define CSC_NONE 0

class ValActControler:public ActControler
{
protected:
    ValActuator* actuator;

    u_int8_t csc_lv=0;
    float from;

    unique_ptr<EMG> emg=nullptr;

    //c++11ではstd::make_uniqueがないので自前
    unique_ptr<EMG> make_unique(EMG* signal){
      return unique_ptr<EMG>(signal);
    }

public:
    ValActControler(ValActuator& actuator_):
        actuator(&actuator_),from(actuator_.get_value())
    {
        id=actuator_.get_id();
    }

    float get_value(){
        return actuator->get_value();
    }

    void loop(){
        if (emg!=nullptr && (emg->get_state()==EMG_RUNNING ||emg->get_state()==EMG_READY)){
            #ifdef DEBUG
            //Serial.printf("act%d start new emg id:%d\n",id,emg->get_id());
            #endif
            actuator->set_value(emg->EMG_update());

            if(emg->get_state()==EMG_DONE){
                actuator->release();
            }
        }
    }

    void add(EMG* sch){
        #ifdef DEBUG
        //Serial.printf("act%d get new emg id:%d csc:%d\n",id,sch->get_id(),sch->get_csclv());
        #endif

        if(emg==nullptr || emg->get_state()==EMG_DONE){
            sch->set_start(actuator->get_value());
            emg=make_unique(sch);
            #ifdef DEBUG
            Serial.printf("act%d start new emg id:%d\n",id,emg->get_id());
            #endif
        }
        else{
            if(sch->get_csclv()>this->emg->get_csclv()){
                sch->set_start(actuator->get_value());
                emg=make_unique(sch);
                #ifdef DEBUG
                Serial.printf("act%d start new emg id:%d\n",id,emg->get_id());
                #endif
            }
        }
    }
};



#endif