#ifndef Motion_h
#define Motion_h

#include <Tween.h>

using std::unique_ptr;

enum value_type{
    ABSOLUTE,
    RELATIVE
};

template<typename T> //typenameとclassに違いはない。基本的なクラスを渡すことを匂わせる
struct frame
{
    int t_ms;//maxだいたい2400秒くらい
    T value;
};


struct Plot{
  std::vector<frame<float>> plots;
};

class IDevice
{
private:
  bool working=false;
public:
  virtual ~IDevice(){}//仮想デストラクタ

  bool isWorking(){
    return working;
  }

  bool allocate(){
    if(working){return false;}
    else{
      working=true;
      return true;
    }
  }

  void release(){
    working=false;
  }
};


class ITimeline{
private:
    
public:
    IDevice* device;
    ITimeline(IDevice* device){
      this->device=device;
    }
    virtual ~ITimeline(){}//仮想デストラクタ
    virtual void start()=0;//純粋仮想関数(abstractみたいな)
    virtual void update()=0;
    virtual void stop()=0;
    virtual bool isRunning()=0;

};



class Motion
{
private:
    std::vector<unique_ptr<ITimeline>> timelines;
    bool isRunning_=false;
    static int construct;
public:
    String name;
    Motion(const std::vector<ITimeline*> timeline,String name="default"){   
        for(int i=0;i<timeline.size();i++){
          this->timelines.push_back(make_unique(timeline[i]));
        }
        this->name=name;
        construct++;
        //Serial.printf("class Motion 残数:%d\n",construct);
    }
    ~Motion(){
      construct--;
    }

    //c++11ではstd::make_uniqueがないので自前
    unique_ptr<ITimeline> make_unique(ITimeline* timeline){
      return unique_ptr<ITimeline>(timeline);
    }

    bool device_occupy(){
      
      for (int i = 0; i < timelines.size(); i++)
      {
        if(timelines[i]->device->isWorking()){
          return false;
        }
      }
      for (int i = 0; i < timelines.size(); i++)
      {
        timelines[i]->device->allocate();
      }
      return true;
    }

    void device_release(){
      for (int i = 0; i < timelines.size(); i++)
      {
        timelines[i]->device->release();
      }
    }

    void start(){
        for (int i=0;i<timelines.size();i++)
        {
            timelines[i]->start();
        }
        isRunning_=true;
        Serial.println("motion start");
        #ifdef BLYNK        
        Blynk.virtualWrite(V11,"motion "+name+"\n");
        #endif
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
            stop();
        }
   
        //DF_state();
    }

    void stop(){
      Serial.println("motion fin");
      device_release();
    }

    bool isRunning(){return isRunning_;}
};
int Motion::construct=0;

enum SetMotionOp{
  EXCLUSIVE,
  WAIT,
  INTERRUPT_  
};

SemaphoreHandle_t device_mux = NULL;//Deviceリソース確保のミューテックス

class MotionPlayer
{
private:
  std::vector<unique_ptr<Motion>> playlist={};
  std::vector<unique_ptr<Motion>> waitlist={};
  std::vector<IDevice*> devices;

  unique_ptr<Motion> make_unique(Motion* motion){
    return unique_ptr<Motion>(motion);
  }

public:
  bool isRunning=false;
  unsigned long pre_motion=0;

  MotionPlayer(std::vector<IDevice*> devices){
    this->devices=devices;
    device_mux = xSemaphoreCreateMutex();
    xSemaphoreGive(device_mux);
  }

  void set_motion(Motion* motion,SetMotionOp option=EXCLUSIVE){

    #ifdef Motion_Debug
    Serial.println("motion set:"+motion->name);
    #endif

    switch(option){
      case EXCLUSIVE:
        if(MotionAvailable(motion)){
          playlist.push_back(make_unique(motion));
          playlist[0]->start();
          isRunning=true;
        }
        else{
          #ifdef Motion_Debug
          Serial.println("another motion proceeding");
          #endif
          delete motion;
        }
        break;
      case WAIT:
        waitlist.push_back(make_unique(motion));
        break;
      case INTERRUPT_:
          delete motion;      
        break;
      default:
          delete motion;
        break;
    }
  }

  bool MotionAvailable(Motion* motion){
    bool ret=false;
    xSemaphoreTake(device_mux, portMAX_DELAY);
    ret=motion->device_occupy();
    xSemaphoreGive(device_mux);
    return ret;
  }

  void motion_task(){
    if (isRunning)
    {
        auto itr=playlist.begin();
        while(itr != playlist.end())//ループ内で要素を削除する
        {
            (*itr)->update();
            if(!(*itr)->isRunning()){

                itr=playlist.erase(itr);
            }
            else{
                ++itr;
            }
        }
        if(playlist.size()==0){
                      
            isRunning=false;
            
            pre_motion=millis();
        }
        
        delayMicroseconds(100);
    }
    else 
    {
      delay(1);
    }
    
  }

  
  
};



#endif
