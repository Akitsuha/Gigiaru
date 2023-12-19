#ifndef EMG_h
#define EMG_h

#include <vector>
#include <memory>
#include "motion_plot.h"
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::string;

enum EMG_state{
    EMG_INIT,
    EMG_READY,
    EMG_RUNNING,
    EMG_PAUSE,
    EMG_DONE,
};

class EMG
{
protected:
    u_int8_t csc_lv=0;
    u_int8_t id=0;
    float from;
    EMG_state state=EMG_INIT;
public:
    EMG(u_int8_t csc_lv,u_int8_t id=0):csc_lv(csc_lv),id(id){}
    virtual ~EMG(){}//仮想デストラクタ

    u_int8_t get_csclv(){
        return csc_lv;
    }
    u_int8_t get_id(){
        return id;
    }
    EMG_state get_state(){
        return state;
    }
    bool set_start(float from){
        #ifdef DEBUG
        Serial.printf("EMG::set_start\n");
        #endif
        this->from=from;
        state=EMG_READY;
        return true;
    }

    virtual motion_t get_duration()=0;
    virtual float EMG_update()=0;
};

class EMG_plot:public EMG{
    vector<Frame_typed> frames={};
    motion_t duration=0;
    double duration_inv=0;
    motion_t start=0;
    vector<Frame_typed>::iterator running_frame;
    float target;

public:
    EMG_plot(const vector<Frame_typed>& frames,u_int8_t csc_lv=1,u_int8_t id=0):EMG(csc_lv,id),frames(frames){
        //[]演算子はconstな参照を返さないのでconstな連想配列には使えない
        if(frames.size()==0){
            duration=0;
        }
        else{
            duration=frames.back().first+frames.back().second->duration;
        }
        
    }

    motion_t get_duration(){
        return duration;
    }

    float EMG_update(){
        float ret=from;
        if(state==EMG_READY){
            if(frames.size()>0){
                running_frame=frames.begin();
                target=(*running_frame).second->get_to(from);
                motion_t f_duration=(*running_frame).second->duration;
                if(f_duration!=0){duration_inv=1./(double)f_duration;}
                start=millis();
                state=EMG_PAUSE;
            }
            else{
                state=EMG_DONE;
            }
        }
        motion_t time=millis()-start;
        if(state==EMG_PAUSE){
            if(running_frame->first <= time){
                state=EMG_RUNNING;
            }
        }
        if(state==EMG_RUNNING){
            ret = (target - from) * easeSineInOut((time-running_frame->first)*duration_inv) + from;
            #ifdef DEBUG
            //Serial.printf("EMG_plot::EMG_update step:%d duration:%d ret:%f\n",step,duration,ret);
            #endif
            if(time >= running_frame->first+running_frame->second->duration){
                running_frame++;
                if(running_frame!=frames.end()){
                    #ifdef DEBUG
                    Serial.printf("Go next frame\n");
                    #endif
                    from=target;
                    target=(*running_frame).second->get_to(from);
                    motion_t f_duration=(*running_frame).second->duration;
                    if(f_duration!=0){duration_inv=1/(double)f_duration;}
                    if(running_frame->first > time){
                        state=EMG_PAUSE;
                    }
                }
                else{
                    #ifdef DEBUG
                    Serial.printf("EMG end\n");
                    #endif
                    state=EMG_DONE;
                }
            }
        }
        
        return ret;
    }
private:
    double easeSineInOut(double t) const
    {
        return -1. / 2. * (cos(M_PI * t / 1.) - 1.);
    }
};

typedef shared_ptr<EMG> EMG_ptr;


#endif