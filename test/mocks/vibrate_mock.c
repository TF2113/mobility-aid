#include "vibrate.h"
#include "unity.h"
#include <stdio.h>
#include <errno.h>

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
    
    char *endptr_count;
    char *endptr_duration;
    char *endptr_delay;
    long count_long;
    double duration, delay;

    errno = 0;

    count_long = strtol(argv[1], &endptr_count, 10);

    if (errno != 0 || endptr_count == argv[1] || *endptr_count != '\0') {
        fprintf(stderr, "Error: 'vibration_count' must be a valid integer.\n");
        return 1;
    }

    duration = strtod(argv[2], &endptr_duration);
    if (endptr_duration == argv[2] || *endptr_duration != '\0') {
        fprintf(stderr, "Error: 'duration' must be a valid number.\n");
        return 1;
    }

    delay = strtod(argv[3], &endptr_delay);
    if (endptr_delay == argv[3] || *endptr_delay != '\0') {
        fprintf(stderr, "Error: 'delay' must be a valid number.\n");
        return 1;
    }
    if (count_long <= 0 || duration < 0 || delay < 0) {
        fprintf(stderr, "Error: count must be positive, duration and delay cannot be negative.\n");
        return 1;
    }

    int count = (int)count_long;

    return vibrate(count, duration, delay);
}