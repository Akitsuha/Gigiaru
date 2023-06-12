#ifndef Motion_Folder_h
#define Motion_Folder_h

#include <math.h>
#include <stdlib.h>
#include "gigi_component.h"
#include "random.h"

/*
Plot_ptr turn(motion_t duration,float angle){
  return make_plotptr(new Plot({{SERVO_ID,{{0,make_frame_ptr(new Frame_Rel(duration,angle))}}}}));//フル尺
}*/

vector<Frame_ptr> shake(motion_t time,float value,int num_repeat){
  vector<Frame_ptr> ret={};
  for(int i=0;i<num_repeat;i++){
    ret.push_back(make_frame_ptr(new Frame_Rel(time,value)));
    ret.push_back(make_frame_ptr(new Frame_Rel(time,-value)));
  }
  return  ret;
}

vector<Frame_ptr> charge(motion_t time,float value){
  return {make_frame_ptr(new Frame_Rel(time/6,-value/50)),make_frame_ptr(new Frame_Rel(time*5/6,value+value/50))};
}

vector<Frame_ptr> shake_r(motion_t time,float value,int num_repeat){
  vector<Frame_ptr> ret={};
  for(int i=0;i<num_repeat;i++){
    ret.push_back(make_frame_ptr(new Frame_Abs(time/6,value)));
    ret.push_back(make_frame_ptr(new Frame_Abs(time*4/6,value)));
    ret.push_back(make_frame_ptr(new Frame_Abs(time/6,0)));
    ret.push_back(make_frame_ptr(new Frame_Abs(time/6,-value)));
    ret.push_back(make_frame_ptr(new Frame_Abs(time*4/6,-value)));
    ret.push_back(make_frame_ptr(new Frame_Abs(time/6,0)));
  }
  return  ret;
}


Plot_ptr joy(){
  return make_plotptr(new Plot({{R_SERVO_ID,shake_r(norm(165,15,1),100,3)}}));
}


#define T_INIT 100
#define T_BEAT 457

#define A_SHAKE_1 30

#define T_S1_ACC 200
vector<Frame_ptr> dance1_servo_frame=
    {make_frame_ptr(new Frame_Rel(T_S1_ACC,A_SHAKE_1)),
    make_frame_ptr(new Frame_Rel(T_BEAT-T_S1_ACC,0)),
    make_frame_ptr(new Frame_Rel(T_S1_ACC,-A_SHAKE_1)),
    make_frame_ptr(new Frame_Rel(T_BEAT-T_S1_ACC,0)),
    make_frame_ptr(new Frame_Rel(T_S1_ACC,A_SHAKE_1)),
    make_frame_ptr(new Frame_Rel(T_BEAT-T_S1_ACC,0)),
    make_frame_ptr(new Frame_Rel(T_S1_ACC,-A_SHAKE_1)),
    make_frame_ptr(new Frame_Rel(T_BEAT-T_S1_ACC,0)),};
Plot_ptr dance1=make_plotptr(new Plot({{SERVO_ID,dance1_servo_frame}}));
Plot_ptr dance2=make_plotptr(new Plot({{R_SERVO_ID,shake_r(T_BEAT/2,70,4)},{SERVO_ID,shake(T_BEAT,15,2)}}));
Plot_ptr dance3=make_plotptr(new Plot({{SERVO_ID,shake(T_BEAT,A_SHAKE_1,2)}}));
Plot_ptr dance41=make_plotptr(new Plot({{SERVO_ID,shake(T_BEAT/2,A_SHAKE_1,2)}}));
Plot_ptr dance42=make_plotptr(new Plot({{R_SERVO_ID,shake_r(T_BEAT/2,70,2)},{SERVO_ID,shake(T_BEAT,15,1)}}));

std::vector<Plot_ptr> plots={dance1,dance2,dance3,dance41,dance42};
Plot_ptr dance=make_plotptr(new Plot(plots));


Plot_ptr charge_plot=make_plotptr(new Plot({{SERVO_ID,charge(1000,60)}}));


#endif