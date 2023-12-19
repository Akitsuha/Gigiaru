#ifndef Debug_h
#define Debug_h

#include "myBlynk.h"

int println(std::string message){
    #ifdef BLYNK 
    terminal.println(message.c_str());
    #else
    Serial.println(message.c_str());
    #endif
    return 0;
}

#endif