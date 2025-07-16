#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

extern int prox_vibrate;
extern int prox_yellow_led;
extern int camera_enabled;

void load_config(const char* filename);

#endif