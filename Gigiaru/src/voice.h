#ifndef Voice_h
#define Voice_h

void voice_setup();
void loadMP3(const char* filename);

void voice();
bool voice_loop();

void voice_loop_task(void *pvParameters);

#endif