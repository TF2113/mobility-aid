#include "vibrate.h"
#include <stdio.h>

// Mock version of the vibrate function.
int vibrate(int count, double duration, double delay) {
    (void)count;
    (void)duration;
    (void)delay;
    return 0; 
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