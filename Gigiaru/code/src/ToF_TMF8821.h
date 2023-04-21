#ifndef ToF_T_h
#define ToF_T_h

#include "SparkFun_TMF882X_Library.h" //http://librarymanager/All#SparkFun_Qwiic_TMPF882X

SparkFun_TMF882X  myTMF882X;

// Structure to hold the measurement results - this is defined by the TMF882X SDK.

static struct tmf882x_msg_meas_results myResults;

void ToFT_setup()
{
    // Initialize the TMF882X device
    if(!myTMF882X.begin())
    {
        Serial.println("Error - The TMF882X failed to initialize - is the board connected?");
        while(1);
    }else
        Serial.println("TMF882X started.");
}

void get_ToF_T()
{
    delay(2000);

    // get a Measurement
    if(myTMF882X.startMeasuring(myResults))
    {
        // print out results
        Serial.println("Measurement:");
        Serial.print("     Result Number: "); Serial.print(myResults.result_num);
        Serial.print("  Number of Results: "); Serial.println(myResults.num_results);       

        for (int i = 0; i < myResults.num_results; ++i) 
        {
            Serial.print("       conf: "); Serial.print(myResults.results[i].confidence);
            Serial.print(" distance mm: "); Serial.print(myResults.results[i].distance_mm);
            Serial.print(" channel: "); Serial.print(myResults.results[i].channel);
            Serial.print(" sub_capture: "); Serial.println(myResults.results[i].sub_capture);   

        }
        Serial.print("     photon: "); Serial.print(myResults.photon_count);    
        Serial.print(" ref photon: "); Serial.print(myResults.ref_photon_count);
        Serial.print(" ALS: "); Serial.println(myResults.ambient_light); Serial.println();

    }

}

#endif