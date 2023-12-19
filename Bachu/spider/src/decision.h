#ifndef Decision_h
#define Decision_h

#include "motion_library.h"
#include "component.h"
#include "myBlynk.h"
#include "motion.h"

class Decision{
protected:
    std::map<int,ValActControler*> actuators;
    ValActControler* servo_ctr;
public:
    Decision(std::map<int,ValActControler*> actuators):
        actuators(actuators),servo_ctr(actuators.at(1)){}
    virtual ~Decision(){}
    virtual void loop()=0;

};


class Decision1:Decision
{
private:
    
public:
    Decision1(std::map<int,ValActControler*> actuators)
        :Decision(actuators){}
    void loop(){
        static u_int16_t pulth=2000;
        static u_int16_t act_time=100;
        static unsigned long pre_step=0;
        static int step=0;
        if(millis()-pre_step>pulth){
            
            //Serial.printf("step:%d\n",step);

            switch(step%2){
                case 0:
                    make_motion_ptr(new Motion(trn_to(SERVO_ID_11,500,30),CSC_1))->start(actuators);
                    break;
                case 1:
                    make_motion_ptr(new Motion(trn_to(SERVO_ID_11,500,60),CSC_1))->start(actuators);
                    break;
            }
            /*
            switch(step%2){
                case 0:
                    make_motion_ptr(new Motion(trn_leg(SERVO_ID_11,SERVO_ID_12,act_time,60,30),CSC_1))->start(actuators);;
                    make_motion_ptr(new Motion(trn_leg(SERVO_ID_31,SERVO_ID_32,act_time,60,30),CSC_1))->start(actuators);;
                    make_motion_ptr(new Motion(trn(SERVO_ID_21,act_time,-60),CSC_1))->start(actuators);;
                    make_motion_ptr(new Motion(trn(SERVO_ID_41,act_time,-60),CSC_1))->start(actuators);;

                    break;
                case 1:
                    make_motion_ptr(new Motion(trn_leg(SERVO_ID_21,SERVO_ID_22,act_time,60,30),CSC_1))->start(actuators);;
                    make_motion_ptr(new Motion(trn_leg(SERVO_ID_41,SERVO_ID_42,act_time,60,30),CSC_1))->start(actuators);;
                    make_motion_ptr(new Motion(trn(SERVO_ID_11,act_time,-60),CSC_1))->start(actuators);;
                    make_motion_ptr(new Motion(trn(SERVO_ID_31,act_time,-60),CSC_1))->start(actuators);;

                    break;

                default:
                    break;
                
            }
            
            switch(step%4){
                case 0:
                    make_motion_ptr(new Motion(trn_leg(SERVO_ID_11,SERVO_ID_12,pulth-10,60,30),CSC_1))->start(actuators);;
                    make_motion_ptr(new Motion(trn_to(SERVO_ID_41,act_time*3-10,0),CSC_1))->start(actuators);;
                    break;
                case 1:
                    make_motion_ptr(new Motion(trn_leg(SERVO_ID_21,SERVO_ID_22,pulth-10,60,30),CSC_1))->start(actuators);;
                    make_motion_ptr(new Motion(trn_to(SERVO_ID_11,act_time*3-10,0),CSC_1))->start(actuators);;
                    break;
                case 2:
                    make_motion_ptr(new Motion(trn_leg(SERVO_ID_31,SERVO_ID_32,pulth-10,60,30),CSC_1))->start(actuators);;
                    make_motion_ptr(new Motion(trn_to(SERVO_ID_21,act_time*3-10,0),CSC_1))->start(actuators);;
                    break;
                case 3:
                    make_motion_ptr(new Motion(trn_leg(SERVO_ID_41,SERVO_ID_42,pulth-10,60,30),CSC_1))->start(actuators);;
                    make_motion_ptr(new Motion(trn_to(SERVO_ID_31,act_time*3-10,0),CSC_1))->start(actuators);;
                    break;

                default:
                    break;
                
            }*/
            pre_step=millis();
            step++;
        }
        
    }
private:
    
};



#endif