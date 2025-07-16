#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "config_loader.h"

int prox_vibrate = 15;      //Default values
int prox_yellow_led = 25;   
int camera_enabled = 1;     

void load_config(const char* filename) {
    FILE *file = fopen(filename, "r");
    if(!file){
        perror("Config file not found");
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)){
        if (sscanf(line, "prox_vibrate=%d", &prox_vibrate)==1) continue;
        if (sscanf(line, "prox_yellow_led=%d", &prox_yellow_led)==1) continue;
        if (sscanf(line, "camera_enabled=%d", &camera_enabled)==1) continue;
    }
}