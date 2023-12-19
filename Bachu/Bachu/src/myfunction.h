#ifndef MyFunction_h
#define  MyFunction_h

#include <random>

std::random_device rnd;     // 非決定的な乱数生成器でシード生成機を生成
std::mt19937 mt(rnd()); //  メルセンヌツイスターの32ビット版、引数は初期シード

//正規分布の乱数
float norm(float avr,float st_dev,float range_l,float range_h){
  std::normal_distribution<> norm(avr, pow(st_dev,2));
  float ret=norm(mt);
  if(ret<range_l){
    ret=range_l;
  }
  else if(ret>range_h){
    ret=range_h;
  }
  return ret;
}

float norm(float avr,float st_dev,int range_times_dev=2){

  return norm(avr,st_dev,avr-range_times_dev*st_dev,avr+range_times_dev*st_dev);
}


template <typename T>
T unsigned_LPF(T& value, const T& new_data,const float& LPF_rate)
{
    T dif=0;
    if(new_data>value)
    {
        dif=new_data-value;
        value+=dif*LPF_rate;
    }
    else if(new_data<value){
        dif=value-new_data;
        value-=dif*LPF_rate;
    }
    return dif;
}

#endif