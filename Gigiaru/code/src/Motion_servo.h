/*#ifndef Motion_Servo_h
#define Motion_Servo_h

#include "motion.h"

class Servo:public IDevice
{
private:
  float setting[16][5];
  int channel;
  
public:
  float angle=0;
  float angle_max=0;
  float default_angle;
  bool motion=false;
  Servo(int pin, int channel, float freq, float bit_num, float time_min, float time_max, float angle_max,float default_angle)
  {
    this->channel=channel;
    this->angle_max=angle_max;
    this->default_angle=default_angle;
    setting[channel][0] = freq;       // 周波数
    setting[channel][1] = bit_num;    // bit数
    setting[channel][2] = time_min;   // 0゜パルス時間ms
    setting[channel][3] = time_max;   // 最大角度パルス時間ms
    setting[channel][4] = angle_max;  // 最大動作角度
    // PWM設定
    pinMode(pin, OUTPUT);               // PWM用ピンを出力設定
    ledcSetup(channel, freq, bit_num);  // PWM設定（ﾁｬﾝﾈﾙ, 周波数, ビット数）
    ledcAttachPin(pin, channel);        // PWMﾁｬﾝﾈﾙ割り当て（ピン番号, ﾁｬﾝﾈﾙ）

    rotate(default_angle);
    release();
  }


  void rotate(float angle_to){

    float resolution;   // 分解能
    float period;       // 周期ms
    float duty_min;     // 0゜Duty計算値
    float duty_max;     // 最大角度Duty計算値
    float duty_active;  // 動作範囲のDuty計算値
    int duty_value;     // PWM出力Duty値（High状態）※整数型として小数点以下切り捨て
    float read_angle;   // PWM出力Duty値からの角度換算

    // サーボ定数算出
    resolution = pow(2, setting[channel][1]); // 分解能（2のbit数乗）
    period = 1000 / setting[channel][0];      // 周期（1000ms / 周波数） 
    duty_min = (resolution / period) * (setting[channel][2]); // 0゜Duty計算値（(分解能/周期)*0゜パルス時間μs）
    duty_max = (resolution / period) * (setting[channel][3]); // 最大角度Duty計算値（(分解能/周期)*最大角度パルス時間μs）
    duty_active = duty_max - duty_min;        // 動作範囲のDuty計算値
    duty_value = (angle_to * (duty_active / setting[channel][4])) + duty_min;  // 角度指定値（浮動小数点数） → PWM出力Duty値（整数）換算

    if (duty_value <= duty_min) { // PWM出力Duty値が0゜Duty計算値以下なら
      duty_value = duty_min + 1;  // PWM出力Duty値＋1
    }
    // PWM出力実行
    ledcWrite(channel, duty_value); // 指定したﾁｬﾝﾈﾙにDuty値を指定してPWM出力実行
    angle=angle_to;
  }

  void release(){
    ledcWrite(channel, 0);
  }

  float get_relative(){
    return angle-default_angle;
  }
};


class TServo:public ITimeline
{
private:
    Servo* servo;
    std::vector<frame<float>> plots;
    float zero;
    
    value_type t_plot_type;
    Tween::Timeline timeline;

public:
    TServo(Servo* servo,Plot plot,value_type t_plot_type=RELATIVE):ITimeline(servo){
        this->servo=servo;
        this->plots=plot.plots;
        this->zero=servo->default_angle;
        this->t_plot_type=t_plot_type;
    }

    void start(){
        this->timeline=Tween::Timeline();
        for(int i=0;i<plots.size(); i++){
        
            if(i==0){
              if(plots[0].t_ms==0){
                timeline.add(servo->angle)
                .init(plots[0].value+zero);
              }else {
                timeline.add(servo->angle)
                .init(servo->angle)
                .then<Ease::Sine>(plots[0].value+zero,plots[0].t_ms);
              }
            }
            else{
                int in_ms;
                switch(t_plot_type){
                  case RELATIVE:
                    in_ms=plots[i].t_ms;
                    break;
                  case ABSOLUTE:
                    in_ms=plots[i].t_ms-plots[i-1].t_ms;
                    break;
                  default:
                    in_ms=500;
                    break;
                }
                int da=plots[i].value-plots[i-1].value;
                if(da==0){
                  timeline[servo->angle].hold(in_ms);
                }
                else{
                  timeline[servo->angle].then<Ease::Sine>(plots[i].value+zero,in_ms);
                }
            }
        }
        timeline.update_duration();
        timeline.start();
    }

    void update(){
      if(!isRunning())
        {
          stop();
        }
      else{
          timeline.update();
          servo->rotate(servo->angle);
          //Serial.printf("%f,%f,%f,%f\n",parts[0].angle,parts[1].angle,parts[2].angle,parts[3].angle);
      }
    }

    void stop(){
      servo->release();
      servo->motion=false;
    }

    bool isRunning(){
        return timeline.isRunning();
    }
};

#endif*/
