#ifndef ULTRA_SENSOR_H
#define ULTRA_SENSOR_H

#include <stdint.h>

float ultrasonic_get_distance_cm(volatile uint32_t *gpio);

#endif