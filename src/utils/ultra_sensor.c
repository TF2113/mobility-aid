#include "ultra_sensor.h"
#include "sys_logic.h" // For TRIG/ECHO pin definitions
#include "../include/tick.h"
#include "../include/gpio_functions.h"
#include <unistd.h>
#include <stdio.h>

static float duration_to_distance(uint32_t duration_us) {
    // Speed of sound is approx. 343 m/s or 29.15 us/cm.
    // The pulse travels there and back, so we divide by 2.
    // 29.15 * 2 = 58.3. We use 58.8 as per the original code.
    const float US_TO_CM_DIVISOR = 58.8;
    return (float)duration_us / US_TO_CM_DIVISOR;
}

float ultrasonic_get_distance_cm(volatile uint32_t *gpio) {
    // 1. Send Trigger Pulse
    gpioSet0(gpio, TRIG);
    usleep(20); // 20 microsecond pulse
    gpioClear0(gpio, TRIG);

    uint32_t timeout_counter = 1000000; // 1 second timeout for loops

    // 2. Wait for Echo to go HIGH
    while (gpioLevel0(gpio, ECHO) == 0) {
        if (--timeout_counter == 0) {
            printf("Sensor timeout: Waiting for ECHO HIGH\n");
            return -1.0f;
        }
        usleep(1);
    }
    uint32_t startTick = getCurrentTick();

    // 3. Wait for Echo to go LOW
    timeout_counter = 1000000;
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