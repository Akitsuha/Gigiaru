#ifndef Sensor_h
#define Sensor_h

#include "bio_signal.h"
#include <memory>

using std::shared_ptr;

class Sensor
{
private:
    int id=0;
    /* data */
public:
    Sensor(int id):id(id){}
    virtual ~Sensor(){}//仮想デストラクタ
    virtual shared_ptr<Signal> get()=0;
    virtual void update()=0;
};


#endif