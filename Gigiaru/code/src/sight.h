#ifndef Sight_h
#define Sight_h

#include <Wire.h>
#include <SparkFun_VL53L5CX_Library.h>
#include "SparkFun_TMF882X_Library.h"
#include "sensor.h"
#include "vector"
#include <memory>


using std::vector;
using std::shared_ptr;

SemaphoreHandle_t xMutex = NULL;//I2C排他制御用のミューテックス

#define UNCERTAIN_VALUE 0

class ToF_Eye:public Sensor{
protected:
    u_int16_t width;
    u_int32_t resolution;
    //正規化された値。わかりやすさのため、遠いと0,近いと255
    vector<u_int8_t> data;
public:
    ToF_Eye(int id,u_int16_t width):width(width),Sensor(id)
    {
        resolution=width*width;
        data.assign(resolution,UNCERTAIN_VALUE);
    }

    shared_ptr<Signal> get(){
        vector<u_int8_t> ret={};
        vector<u_int16_t> composition={width,width};
        return std::make_shared<Signal>(data,composition);
    }
};


SparkFun_TMF882X  myTMF882X;
static struct tmf882x_msg_meas_results myResults;
#define SPAD_MAP 7 //4*4ノーマルのSPADマップID

class TMF8821_Eye:public ToF_Eye{
public:
    TMF8821_Eye(int id,u_int16_t width):ToF_Eye::ToF_Eye(id,width){
        if(!myTMF882X.begin(Wire1))
        {
            Serial.println("Error - The TMF882X failed to initialize - is the board connected?");
            while(1);
        }else
            Serial.println("TMF882X started.");
        myTMF882X.setCurrentSPADMap(SPAD_MAP);
    }
    void update(){
        xSemaphoreTake(xMutex, portMAX_DELAY);
        bool available=myTMF882X.startMeasuring(myResults);
        xSemaphoreGive(xMutex);
        if(available)
        {
            for (int i = 0; i < myResults.num_results; i++) 
            {
                u_int32_t raw=myResults.results[i].distance_mm;
                u_int8_t value=0;
                if(raw<4096)
                {
                    value=255-(u_int8_t)(log2(raw)*21.25);// =255/12
                }
                data[myResults.results[i].sub_capture*8+myResults.results[i].channel-1]=value;
            }
        }
    }

};

SparkFun_VL53L5CX myImager;
VL53L5CX_ResultsData measurementData;
class VL53L5CX_Eye:public ToF_Eye{
private:

public:
    VL53L5CX_Eye(int id,u_int8_t width):ToF_Eye::ToF_Eye(id,width){

        Serial.println();
        Serial.println("setup:VL53L5CX");
       
        if (myImager.begin((DEFAULT_I2C_ADDR >> 1),Wire1) == false)
        {
            Serial.println(F("エラー:"));
            while (1) ;
        }
        
        myImager.setResolution(resolution); //Enable all 64 pads
        myImager.startRanging();
    }
    void update(){      
        xSemaphoreTake(xMutex, portMAX_DELAY);
        bool available=myImager.isDataReady()&&myImager.getRangingData(&measurementData);
        xSemaphoreGive(xMutex);
        if (available)
        {
            //The ST library returns the data transposed from zone mapping shown in datasheet
            //Pretty-print data with increasing y, decreasing x to reflect reality
            //4*4の場合、リストの前半16個に値が入っている
            for(int i=0;i<resolution;i++){
                u_int32_t raw=measurementData.distance_mm[i];
                u_int8_t value=0;
                if(raw<4096)
                {
                    value=255-(u_int8_t)(log2(raw)*21.25);// =255/12
                }
                data[i]=value;
            }
        }
    }
};



/*
void ToF_image(){

    shared_ptr<Signal> T_sig=T_eye->get();
    shared_ptr<Signal> V_sig=V_eye->get();
    vector<u_int8_t> T_image=T_sig->get_raw();
    vector<u_int8_t> V_image=V_sig->get_raw();
    if(T_image.size()==EYE_RESO*EYE_RESO && V_image.size()==EYE_RESO*EYE_RESO){
        for (int y = 0 ; y < EYE_RESO ; y ++)
        {
            for (int x = 0 ; x< EYE_RESO;  x++)
            {
                Serial.print("\t");
                
                if(T_image[x+y*EYE_RESO]<200){
                    //Serial.print("口");
                    Serial.print(T_image[x+y*EYE_RESO]);
                }
                else{
                    //Serial.print("圏");
                    Serial.print(T_image[x+y*EYE_RESO]);
                }
            }
            Serial.print("\t\t");
            for (int x = 0 ; x< EYE_RESO;  x++)
            {
                Serial.print("\t");
                if(V_image[x+y*EYE_RESO]<200){
                    //Serial.print("口");
                    Serial.print(V_image[x+y*EYE_RESO]);
                }
                else{
                    //Serial.print("圏");
                    Serial.print(V_image[x+y*EYE_RESO]);
                }
                //Serial.print(V_image[x+y*EYE_RESO]);
            }
            Serial.println();
        }
        Serial.println();
        u_int8_t T_avr=T_sig->get_avr();
        u_int8_t V_avr=V_sig->get_avr();
        #ifdef BLYNK
        Blynk.virtualWrite(V0,T_avr);
        Blynk.virtualWrite(V7,V_avr);
        #endif
    }

}*/





#endif