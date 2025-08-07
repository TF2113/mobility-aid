#ifndef ULTRA_SENSOR_H
#define ULTRA_SENSOR_H

#include <stdint.h>

float duration_to_distance(uint32_t duration_us);
float ultrasonic_get_distance_cm(volatile uint32_t *gpio);

#endif