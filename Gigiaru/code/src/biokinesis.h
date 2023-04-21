#ifndef Biokinesis_h
#define Biokinesis_h

#include "act_controler.h"

class Biokinesis
{
private:
    ValActControler* servo_ctr;
    ValActControler* r_servo_ctr;
public:
    Biokinesis(ValActControler* servo_ctr,ValActControler* r_servo_ctr):servo_ctr(servo_ctr),r_servo_ctr(r_servo_ctr){}

    void loop(){
        static size_t clock_T=10;
        static float max_S=50;
        static float second_S=25;
        static float third_S=12.5;

        static size_t mspb=500;
        static size_t msprevi=2000;

        static u_int8_t count=0;
        static unsigned long pre_beat=millis();

        static unsigned long pre_revi=millis();

        if(millis()-pre_beat>mspb){

            count=(count+1)%4;
            if(count==0){
                r_servo_ctr->add(new Serial_EMG({
                    new EMG_abs(clock_T,max_S+second_S+third_S),
                    new EMG_abs(clock_T,max_S+second_S+third_S)},
                    CSC_PHYSIC,200));
            }
            else if (count==2)
            {
                r_servo_ctr->add(new Serial_EMG({
                    new EMG_abs(clock_T,second_S+third_S),
                    new EMG_abs(clock_T,second_S+third_S)},
                    CSC_PHYSIC,202));
            }
            else
            {
                r_servo_ctr->add(new Serial_EMG({
                    new EMG_abs(clock_T,third_S),
                    new EMG_abs(clock_T,third_S)},
                    CSC_PHYSIC,201));
            }
            pre_beat=millis();
            
        }


        if(millis()-pre_revi>msprevi){
            float now_a=servo_ctr->get_value();
            float angle_ma=160;
            float k=2; //0<k,k≠1
            float delta=10;

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
            servo_ctr->add(new EMG_abs(300,angle,CSC_PHYSIC,111));
            pre_revi=millis();
        }
    }

};




#endif