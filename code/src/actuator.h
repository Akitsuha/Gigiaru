#ifndef Actuator_h
#define Actuator_h

class Actuator
{
protected:
    u_int8_t id=0;
public:
    Actuator(u_int8_t id=0):id(id){}
    u_int8_t get_id(){
        return id;
    }
    virtual ~Actuator(){};//仮想デストラクタ
    virtual void update()=0;
};

class ValActuator:public Actuator
{
protected:
    
    float min;
    float max;
    float value;
    bool running=false;
public:
    ValActuator(float min,float max,float init,u_int8_t id=0):min(min),max(max),value(init),Actuator(id){}

    float get_value(){
        return value;
    }
    bool set_value(float input){
        #ifdef DEBUG
        //Serial.printf("act %d set value to %f\n",id,input);
        #endif

        if(input<min){
            value=min;
        }
        else if(input>max){
            value=max;
        }
        else{
            value=input;
        }
        #ifdef DEBUG
        //Serial.printf("servo: %f degree\n",value);
        #endif
        update();
        return true;
    }
    virtual void update()=0;
    virtual void release()=0;
};




#endif