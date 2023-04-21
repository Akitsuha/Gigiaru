#ifndef BSignal_h
#define BSignal_h

#include "vector"
#include <numeric>
using std::vector;

class Signal
{
private:
    vector<u_int8_t> raw_data;
    vector<u_int16_t> data_str;
public:
    Signal(vector<u_int8_t> raw,vector<u_int16_t> str):raw_data(raw),data_str(str){}
    vector<u_int8_t> get_raw(){
        return raw_data;
    }
    u_int8_t get_max(){
        return *max_element(raw_data.begin(), raw_data.end());
    }
    u_int8_t get_avr(){
        if(raw_data.size()==0){return 0;}
        return (u_int8_t)(std::accumulate(raw_data.begin(), raw_data.end(),0.0)/raw_data.size());
    }
};


#endif