#include <voice.h>

#include "FS.h"
#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
AudioGeneratorMP3 *mp3;
AudioFileSourceID3 *id3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
#define CONFIG_I2S_BCK_PIN      19
#define CONFIG_I2S_LRCK_PIN     33
#define CONFIG_I2S_DATA_PIN     22
#define FILENAME "/600.mp3" // "/ohayo.mp3"

void loadMP3(const char* filename)
{
  Serial.print("Play:");
  Serial.println(filename);
  file = new AudioFileSourceSPIFFS(filename);
  id3 = new AudioFileSourceID3(file);
  Serial.println("MP3 begin");
  mp3->begin(id3, out);
}

void voice(){
  if (!mp3->isRunning()) {
      loadMP3(FILENAME);
  }
}

void voice_setup(){
    SPIFFS.begin();
    Serial.printf("MP3 playback begins...\n");
    audioLogger = &Serial;
    out = new AudioOutputI2S();
    out->SetPinout(CONFIG_I2S_BCK_PIN, CONFIG_I2S_LRCK_PIN, CONFIG_I2S_DATA_PIN);
    out->SetGain(0.4);
    mp3 = new AudioGeneratorMP3();
}

bool voice_loop(){
  if (!mp3->loop()){
    Serial.println("MP3 task stop");
     mp3->stop();
     return false;
  }
  return true;
}

void voice_loop_task(void *pvParameters){
  while(1){
    if (mp3->isRunning()) {
      Serial.println("MP3 task loop");
      if (!mp3->loop()){
        Serial.println("MP3 task stop");
        mp3->stop();
        
      }
    }
    delay(1); // do not change delay time
  }
}