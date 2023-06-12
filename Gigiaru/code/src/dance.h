#ifndef Dance_h
#define Dance_h

#include "motion.h"
#include <vector>
#include <map>
using std::vector;
using std::map;

class DanceSegment
{
    vector<ActControler*> actuators={};
    vector<EMG_ptr> emgs={};

public:
    DanceSegment(map<uint8_t,ActControler*> actuators,map<uint8_t,EMG_ptr> emgs){
        this->actuators=actuators;
        this->emgs=emgs;
        auto max_element=std::max_element(emgs.begin(),emgs.end(),[](const auto &x,const auto &y)
            {return x.second->get_duration() < y.second->get_duration();});
        size_t max_duration=max_element->second->get_duration();
        for(auto itr=emgs.begin();itr!=emgs.end();emgs++){
            if(itr->second->get_duration()<max_duration){
                /* 空白をaddしようとしたけど、Uniにも挿入するの面倒だった。ポインタごと入れ替えるのめんどい*/
            }
        }
    }

};



#endif