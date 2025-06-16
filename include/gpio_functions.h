#ifndef GPIO_FUNCTIONS_H
#define GPIO_FUNCTIONS_H

#include <stdint.h>

// Set GPIO function (0 = input, 1 = output, 4 = alt0, etc.)
void gpioSetFunction(volatile uint32_t *gpio, int gpioPin, int functionCode);

// Set GPIO pin high
void gpioSet0(volatile uint32_t *gpio, int gpioPin);

// Set GPIO pin low
void gpioClear0(volatile uint32_t *gpio, int gpioPin);

// Read GPIO pin level (returns 0 or 1)
int gpioRead0(volatile uint32_t *gpio, int gpioPin);

#endif