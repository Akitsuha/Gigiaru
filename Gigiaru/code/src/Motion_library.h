#ifndef Motion_Folder_h
#define Motion_Folder_h

#include <math.h>
#include <stdlib.h>
#include <memory.h>
#include "Motion_servo.h"
#include "Motion_r_servo.h"
#include "voice.h"
#include "motion.h"


#define SERVO_PIN 25
#define R_SERVO_PIN 21

Servo servo=Servo(SERVO_PIN,0,50,12,0.5,2.4,160,0);
RServo rservo=RServo(R_SERVO_PIN,2,50,16,4885,65);//デッドバンド4820~4950

using std::unique_ptr;

MotionPlayer mplayer({&servo,&rservo,&speaker});


void joy(){
  int SWING_T=200;
  float SWING_S=200;
  mplayer.set_motion(new Motion(
    {new TRServo(&rservo,{
      {{SWING_T,SWING_S},
      {SWING_T,-SWING_S},
      {SWING_T,SWING_S},
      {SWING_T,-SWING_S},
      {SWING_T,SWING_S},
      {SWING_T,-SWING_S},
      {SWING_T,0}}}),
    new TVoice({{0,"600"}})},
    "joy"));
}


void sad(){
  
  mplayer.set_motion(new Motion({
    new TRServo(&rservo,{
      {{300,60},
      {1000,60},
      {500,0},
      {300,60},
      {1000,60},
      {500,0}}})},
    "sad"));
}

void clock_rot(){
  int clock_T=100;
  int stay_T=800;
  float max_S=300;
  
  mplayer.set_motion(new Motion({
    new TRServo(&rservo,{
      {{clock_T,max_S},
      {clock_T,0},
      {stay_T,0}}})},
    "clock"));
}

void heart(){
  int clock_T=100;
  float max_S=300;
  float second_S=150;
  float third_S=75;;
  
  mplayer.set_motion(new Motion({
    new TRServo(&rservo,{
      {{clock_T,max_S+second_S+third_S},
      {clock_T,0},
      {300,0},
      {clock_T,third_S},
      {clock_T,0},
      {300,0},
      {clock_T,second_S+third_S},
      {clock_T,0},
      {300,0},
      {clock_T,third_S},
      {clock_T,0},
      {300,0}}})},
    "heart"));
}

void charge_rot(){
  int charge_T=300;
  int rot_T=1000;
  float charge_S=400;
  float rot_S=700;
  
  mplayer.set_motion(new Motion({
    new TRServo(&rservo,{
      {{100,-charge_S},
      {charge_T,0},
      {charge_T,rot_S},
      {rot_T,rot_S},
      {300,0}}})},
    "charge"));
}

void anger(){
  float speed=300;
  
  mplayer.set_motion(new Motion({
    new TRServo(&rservo,{
      {{100,300},
      {1500,300},
      {100,0}}})},
    "anger"));
}

void revi_swing(){
  float now_a=servo.angle;
  float angle_ma=servo.angle_max;
  float k=2; //0<k,k≠1
  float delta=10;

  float A=0;
  float angle=0;
  float random=((float)rand())/RAND_MAX; //0~1の一様乱数

  if (now_a<=delta)
  {
    A=pow(delta,1-k)-pow(angle_ma-now_a,1-k);
    angle=now_a+pow(pow(delta,1-k)-A*random,1/(1-k));
  }
  else if (now_a>=angle_ma-delta)
  {
    A=pow(delta,1-k)-pow(now_a,1-k);
    angle=now_a-pow(pow(now_a,1-k)+A*random,1/(1-k));
  }
  else{
    A=2*pow(delta,1-k)-pow(now_a,1-k)-pow(angle_ma-now_a,1-k);
    float c=(pow(delta,1-k)-pow(now_a,1-k))/A;
    if (random<c)
    {
      angle=now_a-pow(A*random+pow(now_a,1-k),1/(1-k));
    }
    else{
      angle=now_a+pow(2*pow(delta,1-k)-A*random-pow(now_a,1-k),1/(1-k));
    }
  }
  
  //Serial.printf("angle:%f,A:%f,random:%f\n",angle,A,random);
  
  mplayer.set_motion(new Motion({
    new TServo(&servo,{{{3000,angle}}})},
    "revi"));
}

void start_motion(int key){
  
  switch(key){
    case 1:
      revi_swing();
      break;
    case 11:
      joy();
      break;
    case 12:
      anger();
      break;
    case 13:
      sad();
      break;
    case 21:
      clock_rot();
      break;
    case 31:
      charge_rot();
      break;

    default:
      Serial.printf("No motion assigned to %d\n",key);
      break;
  }
}

void motion_task(void *pvParameters){
  while(1){
    mplayer.motion_task();
  }
}
#endif