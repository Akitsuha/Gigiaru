#ifndef Decision_h
#define Decision_h

#include "Motion_library.h"
#include "gigi_component.h"
#include "blynk_admin.h"
#include "perception.h"

class Decision{
protected:
    GigiMemory* memory;
    std::map<int,ValActControler*> actuators;
    ValActControler* servo_ctr;
    ValActControler* r_servo_ctr;
public:
    Decision(GigiMemory* memory,std::map<int,ValActControler*> actuators):
        memory(memory),actuators(actuators),servo_ctr(actuators.at(1)),r_servo_ctr(actuators.at(2)){}
    virtual ~Decision(){}
    virtual void loop()=0;

};

class Decision2:Decision
{
private:
    Perception1* perception;
public:
    Decision2(GigiMemory* memory,std::map<int,ValActControler*> actuators,Perception1* perception)
        :Decision(memory,actuators),perception(perception){}
    void loop(){
        static u_int8_t trace_th=50;

        if(perception->get_entropy()>150){
            Motion_ptr motion=make_motion_ptr(new Motion(joy(),CSC_2));
            motion->start(actuators);
        }

        //ε-グリーディ法
        static float epsilon=0.1;
        static float action_occur=0.01;
        if(action_occur > ((float)rand())/RAND_MAX){
            if(epsilon > ((float)rand())/RAND_MAX){//0~1の一様乱数)
                #ifdef BLYNK
                terminal.println("revi");
                #endif
                revi_explore();
            } 
            else{
                float now_a=servo_ctr->get_value();
                float angle=memory->get_valuest_cell();
                if(abs(angle-now_a)<10){
                    angle=now_a;
                }
                #ifdef BLYNK
                //terminal.print("max->");terminal.println(angle);
                #endif
                vector<Frame_ptr> frames={make_frame_ptr(new Frame_Abs(abs(angle-now_a)*50,angle))};
                Motion_ptr motion=make_motion_ptr(new Motion(make_plotptr(new Plot({{SERVO_ID,frames}})),CSC_2));
                motion->start(actuators);
            }
        }
        
    }
private:
    void revi_explore(){
        float now_a=servo_ctr->get_value();
        float angle_ma=160;
        float k=2; //0<k,k≠1
        float delta=20;

        float A=0;
        float angle=0;
        float random=((float)rand())/RAND_MAX; //0~1の一様乱数

        if (now_a<=delta)
        {
            A=pow(delta,1-k)-pow(angle_ma-now_a,1-k);
            angle=now_a+pow(pow(delta,1-k)-A*random,1/(1-k));
        }
        else if (now_a>=angle_ma-delta)
        {
            A=pow(delta,1-k)-pow(now_a,1-k);
            angle=now_a-pow(pow(now_a,1-k)+A*random,1/(1-k));
        }
        else{
            A=2*pow(delta,1-k)-pow(now_a,1-k)-pow(angle_ma-now_a,1-k);
            float c=(pow(delta,1-k)-pow(now_a,1-k))/A;
            if (random<c)
            {
            angle=now_a-pow(A*random+pow(now_a,1-k),1/(1-k));
            }
            else{
            angle=now_a+pow(2*pow(delta,1-k)-A*random-pow(now_a,1-k),1/(1-k));
            }
        }

        vector<Frame_ptr> revi_frames={make_frame_ptr(new Frame_Abs(abs(angle-now_a)*40,angle))};

        Motion_ptr motion=make_motion_ptr(new Motion(make_plotptr(new Plot({{SERVO_ID,revi_frames}})),CSC_2));
        motion->start(actuators);
    }
};


class Decision1:Decision
{
private:
    
    Perception1* perception;
public:
    Decision1(GigiMemory* memory,std::map<int,ValActControler*> actuators,Perception1* perception)
        :Decision(memory,actuators),perception(perception){}
    void loop(){
        static u_int8_t grab_th=200;

        if(perception->get_ToF_avr()>grab_th){
            Motion_ptr motion=make_motion_ptr(new Motion(make_plotptr(new Plot({{SERVO_ID,shake_s(200,60,4)}})),CSC_1));
            motion->start(actuators);
        }
        
    }
private:
    
};



#endif