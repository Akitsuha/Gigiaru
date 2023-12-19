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

Plot_ptr trn(motion_t time,float value_rel){
  return Plot_ptr(SERVO_ID,{make_frame_ptr(new Frame_Rel(time,value_rel))});
}

Plot_ptr trn_to(motion_t time,float value_abs){
  return Plot_ptr(SERVO_ID,{make_frame_ptr(new Frame_Abs(time,value_abs))});
}

Plot_ptr rot(motion_t time,float velocity){
  return Plot_ptr(R_SERVO_ID,{make_frame_ptr(new Frame_Abs(time/6,velocity)),make_frame_ptr(new Frame_Abs(time*4/6,velocity)),make_frame_ptr(new Frame_Abs(time/6,0))});
}

/// @brief timeだけサーボを静止
/// @param time 
/// @return 
Plot_ptr stop_s(motion_t time){
  return Plot_ptr(SERVO_ID,{make_frame_ptr(new Frame_Rel(time,0))});
}

/// @brief timeだけRサーボを静止
/// @param time 
/// @return 
Plot_ptr stop_r(motion_t time){
  return Plot_ptr(R_SERVO_ID,{make_frame_ptr(new Frame_Abs(time,0))});
}


/// @brief サーボモーターの往復。現在値を端に周期time*2でvalue幅を往復
/// @param time 
/// @param value 
/// @param num_repeat 
/// @return 
Plot_ptr shake_s(motion_t time,float value,int num_repeat){
  return (trn(time,value)+trn(time,-value))*num_repeat;
}

vector<Frame_ptr> charge(motion_t time,float value){
  return {make_frame_ptr(new Frame_Rel(time/6,-value/50)),make_frame_ptr(new Frame_Rel(time*5/6,value+value/50))};
}

Plot_ptr shake_r(motion_t time,float velocity,int num_repeat){
  return  (rot(time/2,velocity)+rot(time/2,-velocity))*num_repeat;
}

Plot_ptr wind_step(motion_t time_goback,float range,int num,motion_t time_pause=0){
  motion_t time_go;
  if(time_goback<time_pause){
    time_go=1;
    time_pause=time_goback-2;
  }
  else{
    time_go=(time_goback-time_pause)/2;
  }

  Plot_ptr ret=trn(time_go,range)+stop_s(time_pause);
  
  if(num>2){
    ret+=((trn(time_go*2,-range*2)+stop_s(time_pause)+trn(time_go*2,range*2)+stop_s(time_pause))*((num-1)/2));
  }
  
  if(num%2==0){
    ret+=(trn(time_go*2,-range*2)+stop_s(time_pause)+trn(time_go,range));
  }
  else{
    ret+=trn(time_go,-range);
  }
  return ret;
}

Plot_ptr wind_head(motion_t time_goback,float speed,int num,motion_t time_pause=0){
  motion_t time_go;
  if(time_goback<time_pause){
    time_go=1;
    time_pause=time_goback-2;
  }
  else{
    time_go=(time_goback-time_pause)/2;
  }

  Plot_ptr ret=rot(time_go,speed)+stop_r(time_pause);
  
  if(num>2){
    ret+=((rot(time_go*2,-speed*2)+stop_r(time_pause)+rot(time_go*2,speed*2)+stop_r(time_pause))*((num-1)/2));
  }
  
  if(num%2==0){
    ret+=(rot(time_go*2,-speed*2)+stop_r(time_pause)+rot(time_go,speed));
  }
  else{
    ret+=rot(time_go,-speed);
  }
  return ret;
}

/// @brief ダンスの最後に挿入する。これ入れないと先に終わったアクチュエータが動き出す
/// @return 
Plot_ptr fin(){
  return stop_r(1)+stop_s(1);
}

Plot_ptr clock(motion_t beat,float speed){
  return stop_r(beat/4)+rot(beat/2,speed)+stop_r(beat/4);
}


Plot_ptr joy(){
  return make_plotptr(new Plot({{R_SERVO_ID,shake_r(norm(165,15,1),100,3)}}));
}

Plot_ptr dance(){
  #define T_INIT 100
  #define T_BEAT 457 //一拍の長さ(ms)。四拍子

  #define A_SHAKE_1 30

  #define T_S1_ACC 200
  #define T_2 100
  #define CLOCK_SPEED 60

  Plot_ptr init=trn_to(500,80)+(stop_r(5000)*stop_s(5000));
  Plot_ptr dance1=wind_step(T_BEAT,A_SHAKE_1,4,100)*(clock(T_BEAT,CLOCK_SPEED)*4);
  Plot_ptr dance2=shake_r(T_BEAT,70,4)*(trn(T_2,40)+trn(T_BEAT*4-T_2*2,-80)+trn(T_2,40));
  Plot_ptr dance3=wind_step(T_BEAT,20,4,50)*(clock(T_BEAT,CLOCK_SPEED)*4);
  Plot_ptr dance4=wind_step(T_BEAT,A_SHAKE_1,2)+(shake_r(T_BEAT,70,2));
  Plot_ptr dance5=wind_step(T_BEAT,A_SHAKE_1,4,100)*(clock(T_BEAT,-CLOCK_SPEED)*4);
  Plot_ptr dance6=wind_step(T_BEAT,A_SHAKE_1,4,100)*(clock(T_BEAT,CLOCK_SPEED)*4);
  Plot_ptr dance7=wind_step(T_BEAT*2,50,2,300)*(clock(T_BEAT,-CLOCK_SPEED)*4);
  Plot_ptr dance8=(stop_s(T_BEAT)+wind_step(T_BEAT,30,2)+wind_step(T_BEAT/2,60,6))*(clock(T_BEAT,CLOCK_SPEED)*4);

  Plot_ptr dance=init+dance1+dance2+dance3+dance4+dance5+dance6+dance7+dance8+fin();

  return dance;

}



Plot_ptr charge_plot=make_plotptr(new Plot({{SERVO_ID,charge(1000,60)}}));
//Plot_ptr new_=charge_plot+charge_plot;


#endif