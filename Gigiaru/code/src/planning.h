#ifndef Planning_h
#define Planning_h

#include "act_controler.h"

class Planning
{
private:
    ValActControler* servo_ctr;
public:
    Planning(ValActControler* servo_ctr):servo_ctr(servo_ctr){}
    
    void turn(int duration,float angle){
        servo_ctr->add(new EMG_rel(duration,angle,CSC_INTENTIONAL,100));
    }
    void turn_to(){

    }
};



#endif