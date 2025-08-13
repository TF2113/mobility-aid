#include "feedback.h"
#include "gpio_functions.h"
#include "sys_logic.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

static void responsive_sleep(unsigned int useconds) {
    unsigned int remaining = useconds;
    const unsigned int chunk = 10000; 

    while (remaining > 0 && running) {
        unsigned int sleep_time = (remaining > chunk) ? chunk : remaining;
        usleep(sleep_time);
        remaining -= sleep_time;
    }
}

static void trigger_vibration_and_led(AppState *state, float distance_cm) {
    int numBlink = state->prox_vibrate / distance_cm;

    if (numBlink < 1) numBlink = 1;   
    if (numBlink > 20) numBlink = 20; 

    int delay = 40000 * distance_cm;

    printf("[FEEDBACK] Triggering vibration for distance %.2f cm.\n", distance_cm);

    for (int j = 0; j < numBlink && running; j++) {
        pid_t pid = fork();
        
        if (pid == 0) {
            char count_str[16];
            char duration_str[16];
            char delay_str[16];
            float delay_sec = delay / 1000000.0f;

            snprintf(count_str, sizeof(count_str), "%d", numBlink);
            snprintf(duration_str, sizeof(duration_str), "%.2f", delay_sec);
            snprintf(delay_str, sizeof(delay_str), "%.2f", delay_sec);

            execl("./builds/vibrate", "./builds/vibrate", count_str, duration_str, delay_str, (char*)NULL);

            perror("execl failed");
            _exit(1); 
        
        } else if (pid > 0) {
            gpioSet0(state->gpio, RED_LED);
            responsive_sleep(delay);
            gpioClear0(state->gpio, RED_LED);
        
        } else {
            perror("Fork failed");
        }

        responsive_sleep(delay);
    }
}

void feedback_update(AppState *state, float distance_cm) {
    if (distance_cm < state->prox_yellow_led) {
        gpioSet0(state->gpio, YELLOW_LED);
    } else {
        gpioClear0(state->gpio, YELLOW_LED);
    }

    if (distance_cm < state->prox_vibrate) {
        trigger_vibration_and_led(state, distance_cm);
    }
}