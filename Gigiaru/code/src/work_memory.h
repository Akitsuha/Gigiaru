#ifndef WMemory_h
#define WMemory_h

#include <vector>
using std::vector;

SemaphoreHandle_t xMemoryMutex = NULL;//I2C排他制御用のミューテックス

class Memory
{
protected:
    vector<vector<u_int8_t>> raw={};
public:
    Memory(){
        xMemoryMutex = xSemaphoreCreateMutex();
        xSemaphoreGive(xMemoryMutex);
    }
    const vector<vector<u_int8_t>>& get(){
        return raw;
    }
};



#endif