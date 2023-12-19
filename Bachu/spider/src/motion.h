#ifndef Motion_h
#define Motion_h

#include <memory>
#include "act_controller.h"
#include "motion_plot.h"

using std::shared_ptr;

class Motion{
    Plot_ptr plot;
    u_int8_t csclv=0; 

public:
    Motion(const Plot_ptr& plot,u_int8_t csclv):plot(plot),csclv(csclv){}

    bool start(std::map<int,ValActControler*>& actuators){
        bool ret=false;
        const std::map<int,vector<Frame_typed>>& frames=plot->get_frame();
        for(auto itr=frames.begin();itr!=frames.end();itr++){
            if(actuators.find(itr->first)==actuators.end()){
                continue;
            }
            ret|=actuators[itr->first]->add(new EMG_plot(itr->second,csclv));
        }
        return ret;
    }

    Plot_ptr get_plot(){
        return plot;
    }
};

typedef shared_ptr<Motion> Motion_ptr;
Motion_ptr make_motion_ptr(Motion* motion){
    return shared_ptr<Motion>(motion);
}


#endif