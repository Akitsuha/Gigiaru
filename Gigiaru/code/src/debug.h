#ifndef Debug_h
#define Debug_h

#include "blynk_admin.h"

int println(std::string message){
    #ifdef BLYNK 
    terminal.println(message.c_str());
    #else
    Serial.println(message);
    #endif
    return 0;
}

#endif