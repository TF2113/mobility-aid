#include "ultra_sensor.h"
#include "sys_logic.h" // For TRIG/ECHO pin definitions
#include "tick.h"
#include "gpio_functions.h"
#include <unistd.h>
#include <stdio.h>

#ifdef TEST
#define SENSOR_TIMEOUT_ITERATIONS 50 
#else
#define SENSOR_TIMEOUT_ITERATIONS 1000000
#endif

float duration_to_distance(uint32_t duration_us) {
    const float US_TO_CM_DIVISOR = 58.8;
    return (float)duration_us / US_TO_CM_DIVISOR;
}

float ultrasonic_get_distance_cm(volatile uint32_t *gpio) {
    // 1. Send Trigger Pulse
    gpioSet0(gpio, TRIG);
    usleep(20);
    gpioClear0(gpio, TRIG);

    uint32_t timeout_counter = SENSOR_TIMEOUT_ITERATIONS;

    // 2. Wait for Echo to go HIGH
    while (gpioLevel0(gpio, ECHO) == 0) {
        if (--timeout_counter == 0) {
            printf("Sensor timeout: Waiting for ECHO HIGH\n");
            return -1.0f;
        }
        usleep(1);
    }
    uint32_t startTick = getCurrentTick();

    // Reset timeout for the second loop
    timeout_counter = SENSOR_TIMEOUT_ITERATIONS;

    // 3. Wait for Echo to go LOW
    while (gpioLevel0(gpio, ECHO) != 0) {
        if (--timeout_counter == 0) {
            printf("Sensor timeout: Waiting for ECHO LOW\n");
            return -2.0f;
        }
        usleep(1);
    }
    uint32_t endTick = getCurrentTick();

    // 4. Calculate and return distance
    uint32_t duration = endTick - startTick;
    return duration_to_distance(duration);
}