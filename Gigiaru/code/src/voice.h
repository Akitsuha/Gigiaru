#ifndef Voice_h
#define Voice_h

#include "motion.h"

#include  <string.h>
#include <driver/i2s.h>
#include "FS.h"
#include "SPIFFS.h"
#include "AudioFileSourceSPIFFS.h"
#include "AudioFileSourceID3.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
using std::string;
AudioGeneratorMP3 *mp3;
AudioFileSourceID3 *id3;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;
#define CONFIG_I2S_BCK_PIN      19
#define CONFIG_I2S_LRCK_PIN     33
#define CONFIG_I2S_DATA_PIN     22
#define CONFIG_I2S_DATA_IN_PIN 23
#define FILENAME "/600.mp3" // "/ohayo.mp3"

#define SPEAKER_I2S_NUMBER I2S_NUM_0

#define MODE_MIC 0
#define MODE_SPK 1
#define DATA_SIZE 2000
#define I2S_BUFFER_COUNT            2//(1,216)でリブートした。小さすぎるとダメ？
#define I2S_BUFFER_SIZE             128

void InitI2SSpeakerOrMic(int mode)
{
    esp_err_t err = ESP_OK;

    i2s_driver_uninstall(SPEAKER_I2S_NUMBER);
    i2s_config_t i2s_config = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER),
        .sample_rate = 16000,
        .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
        .channel_format = I2S_CHANNEL_FMT_ALL_RIGHT,
        .communication_format = I2S_COMM_FORMAT_I2S,
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
        .dma_buf_count = I2S_BUFFER_COUNT,
        .dma_buf_len = I2S_BUFFER_SIZE,
        .use_apll             = false,
        .tx_desc_auto_clear   = false,
        .fixed_mclk           = 0
    };
    if (mode == MODE_MIC)
    {
        i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_PDM);
    }
    else
    {
        i2s_config.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
        i2s_config.use_apll = false;
        i2s_config.tx_desc_auto_clear = true;
    }

    err += i2s_driver_install(SPEAKER_I2S_NUMBER, &i2s_config, 0, NULL);
    i2s_pin_config_t tx_pin_config;

    tx_pin_config.bck_io_num = CONFIG_I2S_BCK_PIN;
    tx_pin_config.ws_io_num = CONFIG_I2S_LRCK_PIN;
    tx_pin_config.data_out_num = CONFIG_I2S_DATA_PIN;
    tx_pin_config.data_in_num = CONFIG_I2S_DATA_IN_PIN;
    tx_pin_config.mck_io_num = GPIO_NUM_0;//なんかたした

    //Serial.println("Init i2s_set_pin");
    err += i2s_set_pin(SPEAKER_I2S_NUMBER, &tx_pin_config);
    //Serial.println("Init i2s_set_clk");
    err += i2s_set_clk(SPEAKER_I2S_NUMBER, 16000, I2S_BITS_PER_SAMPLE_16BIT, I2S_CHANNEL_MONO);
}

//バッファサイズに関わらず100msくらいかかってるっぽい
void hear(){
    static float power=0;
    static float center=0;
    
    size_t byte_read;
    char read_buff[DATA_SIZE];
    i2s_read(SPEAKER_I2S_NUMBER, (char *)read_buff, DATA_SIZE, &byte_read, portMAX_DELAY);
    uint16_t* u16buf=(uint16_t*)&read_buff[0];
    for(int i=0;i<DATA_SIZE/2;i++){
        int16_t raw=(int16_t)(u16buf[i]);
        power+=0.005*((raw-center)*(raw-center)-power);
        center+=0.01*(raw-center);
        //Serial.println(raw);
        float pressure=20 * log10(sqrt(power));
        if (i%100==0)
        {
            //Serial.printf("raw:%d,center:%f\n",raw,center);
            Serial.println(pressure);
        }
        
        
    }
    
}

class Speaker:public IDevice
{
private:
    /* data */
public:

};

Speaker speaker;


void loadMP3(string filename)
{
  file = new AudioFileSourceSPIFFS(filename.c_str());
  id3 = new AudioFileSourceID3(file);
  Serial.println("MP3 begin");
  mp3->begin(id3, out);
}

void voice(string file){
  if (!mp3->isRunning()) {
      loadMP3("/"+file+".mp3");
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

class TVoice:public ITimeline{
private:
    std::vector<frame<string>> timeline;
    int idx=0;
    unsigned long start_us=0;
    bool last=false;
    bool _isRunning=false;
    bool isPlaying=false;

public:
    TVoice(std::vector<frame<string>> timeline):ITimeline(&speaker){
        this->timeline=timeline;
    }

    void start(){
        idx=0;
        start_us=micros();
        last=false;
        _isRunning=true;
    }

    void update(){
      if(_isRunning){
        if (isPlaying)
        {
            if (!voice_loop()){
                isPlaying=false;
                if(last){
                    stop();
                }
            }
        }
        else if(timeline[idx].t_ms*1000 < micros()-start_us){
            isPlaying=true;
            voice(timeline[idx].value);
            idx++;
            if (idx==timeline.size())
            {
                last=true;
            }
        }
      }
    }

    void stop(){
        _isRunning=false;
        isPlaying=false;
    }

    bool isRunning(){
      return _isRunning;
    }

};

#endif