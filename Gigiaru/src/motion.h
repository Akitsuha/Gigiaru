#ifndef Motion_h
#define Motion_h

#include <SmoothStepper.h>
#include <Tween.h>
#include <voice.h>

enum value_t{
    ABSOLUTE,
    RELATIVE
};

struct step{
    value_t type;
    int step;
};

template<typename T> //typenameとclassに違いはない。基本的なクラスを渡すことを匂わせる
struct frame
{
    int t_ms;//だいたい2400秒くらい
    T value;
};

class ITimeline{
public:
    //~ITimeline(){}//仮想デストラクタ
    virtual void start()=0;//純粋仮想関数(abstractみたいな)
    virtual void update()=0;
    virtual bool isRunning()=0;
};

class TServo:public ITimeline
{
private:
    std::function<void(float)> fn;
    std::vector<frame<float>> plots;
    float var;
    Tween::Timeline timeline;

public:
    TServo(float& var,std::function<void(float)> fn,std::vector<frame<float>> plots){
        this->var=var;
        this->fn=fn;
        this->plots=plots;
    }

    void start(){
        this->timeline=Tween::Timeline();
        for(int i=0;i<plots.size(); i++){
        
            if(i==0){
                timeline.add(var)
                .init(var)
                .then<Ease::Sine>(plots[0].value,plots[0].t_ms);
            }
            else{
                int in_ms=plots[i].t_ms-plots[i-1].t_ms;
                //int da=trace[param][i][1]-trace[param][i-1][1];
                //Serial.printf("(%d degree,%d ms)\n",da,in_ms);
                timeline[var].then<Ease::Sine>(plots[i].value,plots[i].t_ms);
            }
        }
        timeline.update_duration();
        timeline.start();
    }

    void update(){
            if(!timeline.isRunning())
        {
            //サーボモータ落とす
            //Serial.printf("駆動時間:%dms",millis()-start_time);
            Serial.println("timeline release");
        }
        else{
            timeline.update();
            fn(var);
            //Serial.printf("%f,%f,%f,%f\n",parts[0].angle,parts[1].angle,parts[2].angle,parts[3].angle);
        }
    }

    bool isRunning(){
        return timeline.isRunning();
    }
};

class TStepper:public ITimeline{
private:
    SStepper* stepper;
    std::vector<frame<step>> timeline;
    int idx=0;
    unsigned long start_us=0;
    int direction=0;            // Direction of rotation
    unsigned long step_delay=1000; // delay between steps, in us, based on speed
    int number_of_steps;      // total number of steps this motor can take
    int steps_left=0;
    int acc_step=60;          //加減速に要するstep数
    int acc=1;
    unsigned long last_step_time=0; // time stamp in us of when the last step was taken
    bool isMoving=false;
    bool isRunning_=false;
    
public:
    TStepper(SStepper* stepper,const std::vector<frame<step>> timeline,int a)
    {
        this->timeline=timeline;
        this->stepper=stepper;
        this->number_of_steps=a;
    }

    void start(){
        idx=0;
        start_us=micros();
        last_step_time=start_us;
        isRunning_=true;
    }

    void update(){
        if(!isRunning_){return;}

        unsigned long now = micros();
        if(!isMoving){
            
            if(timeline[idx].t_ms*1000<=now-start_us){
                int steps=0;
                switch (timeline[idx].value.type)
                {
                case ABSOLUTE:
                    steps=timeline[idx].value.step-(stepper->step_number);
                    steps_left = abs(steps);  // how many steps to take
                    // determine direction based on whether steps_to_mode is + or -:
                    if (steps > 0) { this->direction = 1; }
                    if (steps < 0) { this->direction = 0; }
                    break;
                
                case RELATIVE:
                    steps=timeline[idx].value.step;
                    steps_left = abs(steps);  // how many steps to take
                    // determine direction based on whether steps_to_mode is + or -:
                    if (steps > 0) { this->direction = 1; }
                    if (steps < 0) { this->direction = 0; }
                    break;
                default:
                    break;
                }
                acc=1;
                step_delay=1000*acc_step/stepper->max_speed_spms/acc;
                isMoving=true;
            }
        }
        else
        {
             if (now - last_step_time >= step_delay)
            {
                // get the timeStamp of when you stepped:
                last_step_time = now;
                // increment or decrement the step number,
                // depending on direction:
                if (this->direction == 1)
                {
                    stepper->step_number++;
                }
                else
                {
                    stepper->step_number--;
                }

                stepper->stepMotor(stepper->step_number % 4);

                // 減速処理
                if (acc==steps_left or steps_left<acc_step)
                {
                    this->step_delay=1000*acc_step/stepper->max_speed_spms/steps_left;
                }
                //加速処理
                else if (acc<acc_step)
                {
                    acc++;
                    this->step_delay=1000*acc_step/stepper->max_speed_spms/acc;
                }

                // decrement the steps left:
                steps_left--;

                if(steps_left==0)
                {
                    stepper->sleep();
                    isMoving=false;

                    if(idx==timeline.size()-1)
                    {
                        isRunning_=false;
                    }
                    else{idx++;}
                }
            }
        }
    }

    bool isRunning(){
        return isRunning_;
    }       
};

class TVoice:public ITimeline{
private:
    std::vector<frame<char*>> timeline;
    int idx=0;
    unsigned long start_us=0;
    bool isPlaying=false;
    bool isRunning_=false;
public:
    TVoice(std::vector<frame<char*>> timeline){
        this->timeline=timeline;
    }

    void start(){
        idx=0;
        start_us=micros();
        isRunning_=true;
    }

    void update(){
        if(!isPlaying){
            if(timeline[idx].t_ms*1000<micros()-start_us){
                voice();
                idx++;
                isPlaying=true;
            }
        }
        else{
            isPlaying=voice_loop();
            if (!isPlaying && idx==timeline.size())
            {
                isRunning_=false;
            }
            
        }
    }

    bool isRunning(){
        return isRunning_;
    }

};

class motion
{
private:
    std::vector<ITimeline*> timelines;
    bool isRunning_=false;
public:
    motion(const std::vector<ITimeline*> timelines){
        this->timelines=timelines;
    }

    void start(){
        for (int i=0;i<timelines.size();i++)
        {
            timelines[i]->start();
        }
        isRunning_=true;
    }

    void update(){
        if(!isRunning_){return;}
        bool any=false;
        for (int i=0;i<timelines.size();i++)
        {
            if (timelines[i]->isRunning())
            {
                timelines[i]->update();
                any=true;
            }
            
        }
        if (!any)
        {
            isRunning_=false;
        }
    }

    bool isRunning(){return isRunning_;}
};

motion* motion_;
bool new_motion=false;
bool isRunning=false;

void set_motion(motion* m){
    motion_=m;
    new_motion=true;
}

void motion_task(void *pvParameters){
  while(1){
    if (isRunning)
    {
        motion_->update();
        isRunning=motion_->isRunning();
    }
    else if (new_motion)
    {
        motion_->start();
        isRunning=true;
        new_motion=false;
    }
    delayMicroseconds(10);
  }
}

#endif
