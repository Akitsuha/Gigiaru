#ifndef Motion_h
#define Motion_h

#include <memory>
#include "act_controler.h"

using std::shared_ptr;

class Motion{
    EMG_ptr servo_motion=nullptr;
    EMG_ptr r_servo_motion=nullptr;

public:
    Motion(EMG* servo_motion=nullptr,EMG* r_servo_motion=nullptr)
    {
        if(servo_motion!=nullptr){
            this->servo_motion=make_ptr(servo_motion);
        }
        if(r_servo_motion!=nullptr){
            this->r_servo_motion=make_ptr(r_servo_motion);
        }
    }

    bool start(ValActControler* servo_ctr,ValActControler* r_servo_ctr){
        bool ret=false;
        if(servo_motion!=nullptr){
            ret=(ret||servo_ctr->add(servo_motion));
        }
        if(r_servo_motion!=nullptr){
            ret=(ret||r_servo_ctr->add(r_servo_motion));
        }
        return ret;
    }

protected:
    EMG_ptr make_ptr(EMG* emg){
        return shared_ptr<EMG>(emg);
    }
};

typedef shared_ptr<Motion> Motion_ptr;
Motion_ptr make_motion_ptr(Motion* motion){
    return shared_ptr<Motion>(motion);
}


#endif