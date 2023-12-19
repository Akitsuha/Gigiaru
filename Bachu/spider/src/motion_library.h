#ifndef Motion_Folder_h
#define Motion_Folder_h

#include <math.h>
#include <stdlib.h>
#include "component.h"
#include "myfunction.h"

/*
Plot_ptr turn(motion_t duration,float angle){
  return make_plotptr(new Plot({{SERVO_ID,{{0,make_frame_ptr(new Frame_Rel(duration,angle))}}}}));//フル尺
}*/

Plot_ptr trn(int ID,motion_t time,float value_rel){
  return Plot_ptr(ID,{make_frame_ptr(new Frame_Rel(time,value_rel))});
}

Plot_ptr trn_to(int ID,motion_t time,float value_abs){
  return Plot_ptr(ID,{make_frame_ptr(new Frame_Abs(time,value_abs))});
}

/// @brief timeだけサーボを静止
/// @param time 
/// @return 
Plot_ptr stop(int ID,motion_t time){
  return Plot_ptr(ID,{make_frame_ptr(new Frame_Rel(time,0))});
}

///
Plot_ptr trn_leg(int ID_1,int ID_2,motion_t time,float angle_trn,float angle_hight){
  return trn_to(ID_2,10,angle_hight)+trn_to(ID_1,time-20,angle_trn)+trn_to(ID_2,10,0);
}

//Plot_ptr new_=charge_plot+charge_plot;


#endif