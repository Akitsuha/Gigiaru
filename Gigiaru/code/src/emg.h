#ifndef EMG_h
#define EMG_h

#include <vector>
#include <memory>
using std::vector;
using std::unique_ptr;
using std::string;

enum EMG_state{
    EMG_INIT,
    EMG_READY,
    EMG_RUNNING,
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
        this->from=from;
        state=EMG_READY;
        return true;
    }

    virtual float EMG_update()=0;
};

class Uni_EMG:public EMG{
protected:
    size_t duration;
    float duration_inv=0;
    size_t step=0;
    double easeSineInOut(double t) const
    {
        return -1. / 2. * (cos(M_PI * t / 1.) - 1.);
    }
public:
    Uni_EMG(int duration,u_int8_t csc_lv,u_int8_t id=0):duration(duration),EMG(csc_lv,id)
    {
        if(duration!=0){duration_inv=1/(float)duration;}
    }
    virtual float target(float from)=0;
    float EMG_update(){
        float ret=0;
        if(state==EMG_READY){
            #ifdef DEBUG
            Serial.printf("next Uniemg id:%d %f->%f\n",id,from,target(from));
            #endif
            state=EMG_RUNNING;
        }
        if(state==EMG_RUNNING){
            step++;
            ret = (target(from) - from) * easeSineInOut(step*duration_inv) + from;
            if(step>=duration){
                state=EMG_DONE;
            }
        }
        #ifdef DEBUG
        //Serial.printf("from:%f,target:%f,")
        #endif
        return ret;
    }
};

class EMG_abs:public Uni_EMG{
    float to;
public:
    EMG_abs(size_t duration,float to,u_int8_t csc_lv=1,u_int8_t id=0):to(to),Uni_EMG(duration,csc_lv,id){}

    float target(float from){
        return to;
    }
};

class EMG_rel:public Uni_EMG{
    float dif;
public:
    EMG_rel(size_t duration,float dif,u_int8_t csc_lv=1,u_int8_t id=0):dif(dif),Uni_EMG(duration,csc_lv,id){}

    float target(float from){
        return from+dif;
    }
};

class Serial_EMG:public EMG
{
private:
    vector<unique_ptr<Uni_EMG>> emgs={};
    vector<unique_ptr<Uni_EMG>>::iterator running_emg;
    float target;
    //c++11ではstd::make_uniqueがないので自前
    unique_ptr<Uni_EMG> make_unique(Uni_EMG* signal){
      return unique_ptr<Uni_EMG>(signal);
    }
public:
    Serial_EMG(vector<Uni_EMG*> emgs,u_int8_t csc_lv,u_int8_t id=0):EMG(csc_lv,id){
        for (int i = 0; i < emgs.size(); i++)
        {
            this->emgs.push_back(make_unique(emgs[i]));
        }
    }

    float EMG_update(){
        float ret=0;
        if(state==EMG_READY){
            if(emgs.size()>0){
                #ifdef DEBUG
                Serial.printf("next Serialemg id:%d\n",id);
                #endif
                running_emg=emgs.begin();
                (*running_emg)->set_start(from);
                target=(*running_emg)->target(from);
                state=EMG_RUNNING;
            }
            else{
                state=EMG_DONE;
            }
        }
        if(state==EMG_RUNNING){
            ret=(*running_emg)->EMG_update();
            if((*running_emg)->get_state()==EMG_DONE){
                running_emg++;
                if(running_emg!=emgs.end()){
                    #ifdef DEBUG
                    Serial.printf("Serialemg go next\n");
                    #endif
                    from=target;
                    (*running_emg)->set_start(from);
                    target=(*running_emg)->target(from);
                }
                else{
                    #ifdef DEBUG
                    Serial.printf("Serialemg end\n");
                    #endif
                    state=EMG_DONE;
                }
            }
        }
        return ret;
    }
};


#endif