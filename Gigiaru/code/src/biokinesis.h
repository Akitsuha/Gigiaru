#ifndef Biokinesis_h
#define Biokinesis_h

#include "act_controler.h"

#define SERVO_ID_b 1
#define R_SERVO_ID_b 2

class Biokinesis
{
private:
    ValActControler* servo_ctr;
    ValActControler* r_servo_ctr;
public:
    Biokinesis(ValActControler* servo_ctr,ValActControler* r_servo_ctr):servo_ctr(servo_ctr),r_servo_ctr(r_servo_ctr){}

    void loop(){
        static size_t mspb=1200;
        static unsigned long pre_beat=millis();
        if(millis()-pre_beat>mspb){
            heartbeat();
            pre_beat=millis();
        }
    }

private:
    void heartbeat(){
        static motion_t clock_acc_T=50;
        static motion_t clock_rot_T=200;
        static float max_S=60;
        static float second_S=30;
        static float third_S=15;

        static u_int8_t count=0;

        static Plot_ptr beat123=make_plotptr(new Plot({{R_SERVO_ID_b,
                    {{0,make_frame_ptr(new Frame_Abs(clock_acc_T,max_S+second_S+third_S))},
                    {clock_acc_T,make_frame_ptr(new Frame_Abs(clock_rot_T,max_S+second_S+third_S))},
                    {clock_acc_T+clock_rot_T,make_frame_ptr(new Frame_Abs(clock_acc_T,0))}}}}));

        static Plot_ptr beat23=make_plotptr(new Plot({{R_SERVO_ID_b,
                    {{0,make_frame_ptr(new Frame_Abs(clock_acc_T,second_S+third_S))},
                    {clock_acc_T,make_frame_ptr(new Frame_Abs(clock_rot_T,second_S+third_S))},
                    {clock_acc_T+clock_rot_T,make_frame_ptr(new Frame_Abs(clock_acc_T,0))}}}}));

        static Plot_ptr beat3=make_plotptr(new Plot({{R_SERVO_ID_b,
                    {{0,make_frame_ptr(new Frame_Abs(clock_acc_T,third_S))},
                    {clock_acc_T,make_frame_ptr(new Frame_Abs(clock_rot_T,third_S))},
                    {clock_acc_T+clock_rot_T,make_frame_ptr(new Frame_Abs(clock_acc_T,0))}}}}));

        count=(count+1)%4;
        if(count==0){

            r_servo_ctr->add(new EMG_plot(beat123->get_frame().at(R_SERVO_ID_b),CSC_PHYSIC));
        }
        else if (count==2)
        {
            r_servo_ctr->add(new EMG_plot(beat23->get_frame().at(R_SERVO_ID_b),CSC_PHYSIC));
        }
        else
        {
            r_servo_ctr->add(new EMG_plot(beat3->get_frame().at(R_SERVO_ID_b),CSC_PHYSIC));
        }
        
    }

};




#endif