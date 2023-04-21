#ifndef Sensor_h
#define Sensor_h

#include "bio_signal.h"
#include <memory>

using std::shared_ptr;

class Sensor
{
private:
    /* data */
public:
    virtual ~Sensor(){}//仮想デストラクタ
    virtual shared_ptr<Signal> get()=0;
    virtual void update()=0;
};


#endif