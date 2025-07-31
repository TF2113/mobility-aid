#include "vibrate.h"
#include <stdio.h>

static int s_call_count = 0;
static int s_last_count = 0;
static double s_last_duration = 0.0;
static double s_last_delay = 0.0;

// Mock version of the vibrate function.
int vibrate(int count, double duration, double delay) {
    s_call_count++;
    s_last_count = count;
    s_last_duration = duration;
    s_last_delay = delay;
    return 0; 
}

void reset_vibrate_mock(void) {
    s_call_count = 0;
    s_last_count = 0;
    s_last_duration = 0.0;
    s_last_delay = 0.0;
}

int get_vibrate_call_count(void) {
    return s_call_count;
}

int get_last_vibration_count_arg(void) {
    return s_last_count;
}

double get_last_duration_arg(void) {
    return s_last_duration;
}

double get_last_delay_arg(void) {
    return s_last_delay;
}

int cli_vibrate(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <vibration_count> <duration(s)> <delay(s)>\n", argv[0]);
        return 1;
    }

    int count = atoi(argv[1]);
    double duration = atof(argv[2]);
    double delay = atof(argv[3]);
    if (count <= 0 || duration < 0 || delay < 0) {
        fprintf(stderr, "Error: count, duration and delay must be positive.\n");
        return 1;
    }

    return vibrate(count, duration, delay);
}