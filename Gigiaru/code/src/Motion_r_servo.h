/*#ifndef Motion_R_Servo_h
#define Motion_R_Servo_h

#include "motion.h"

static float rs_setting[16][2]={}; // チャンネル[0〜15]ごとの[周波数, bit数, 0゜パルス時間ms, 最大角度パルス時間ms,最大動作角度]格納配列

class RServo:public IDevice
{
private:
  float setting[16][5];
  int channel;
  
public:
  int pwm=4950;
  float speed=0;
  int default_pwm;
  int deadband;
  bool motion=false;
  RServo(int pin, int channel, float freq, float bit_num,int default_pwm,int deadband)
  {
    this->channel=channel;
    this->default_pwm=default_pwm;
    this->deadband=deadband;
    rs_setting[channel][0] = freq;       // 周波数
    rs_setting[channel][1] = bit_num;    // bit数

    // PWM設定
    pinMode(pin, OUTPUT);               // PWM用ピンを出力設定
    ledcSetup(channel, freq, bit_num);  // PWM設定（ﾁｬﾝﾈﾙ, 周波数, ビット数）
    ledcAttachPin(pin, channel);        // PWMﾁｬﾝﾈﾙ割り当て（ピン番号, ﾁｬﾝﾈﾙ）
  }

  void rotate(int pwm){
    // PWM出力実行
    this->pwm=pwm;
    ledcWrite(channel, pwm); // 指定したﾁｬﾝﾈﾙにDuty値を指定してPWM出力実行
  }

  void release(){
    ledcWrite(channel, 0);
  }
};

class TRServo:public ITimeline
{
private:
    RServo* servo;
    std::vector<frame<float>> plots;
    float dead_sup;
    float dead_inf;
    value_type t_plot_type;
    Tween::Timeline timeline;

public:
    TRServo(RServo* servo,Plot plot,value_type t_plot_type=RELATIVE):ITimeline(servo){
        this->servo=servo;
        this->plots=plot.plots;
        this->dead_sup=servo->default_pwm-servo->deadband;
        this->dead_inf=servo->default_pwm+servo->deadband;
        this->t_plot_type=t_plot_type;
    }

    void start(){
        this->timeline=Tween::Timeline();
        for(int i=0;i<plots.size(); i++){
        
            if(i==0){
              if(plots[0].t_ms==0){
                timeline.add(servo->speed)
                .init(plots[0].value);
              }else {
                timeline.add(servo->speed)
                .init(servo->speed)
                .then<Ease::Sine>(plots[0].value,plots[0].t_ms);
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
                  timeline[servo->speed].hold(in_ms);
                }
                else{
                  timeline[servo->speed].then<Ease::Sine>(plots[i].value,in_ms);
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
          servo->rotate(db_crr(servo->speed));
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

    int db_crr(float s){
      if (s>0)
      {
        return (int)(dead_inf+s);
      }
      else if (s<0)
      {
        return (int)(dead_sup+s);
      }
      else{
        return servo->default_pwm;
      }
    }
};

#endif*/
