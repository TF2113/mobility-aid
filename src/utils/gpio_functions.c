#include <stdint.h>

void gpioSetFunction(volatile uint32_t *gpio, int gpioPin, int functionCode)
{
    int regIndex = gpioPin / 10;        //Either 1 or 2 for Raspberry Pi 4 Pins
    int regBit = (gpioPin % 10) * 3;    //Gets starting bit of register for the pin

    gpio[regIndex] &= ~(7 << regBit);
    gpio[regIndex] |= (functionCode << regBit);
}

void gpioSet0(volatile uint32_t *gpio, int gpioPin){
    gpio[0x1c / 4] = (1 << gpioPin); //GPSET0
}

void gpioClear0(volatile uint32_t *gpio, int gpioPin){
    gpio[0x28 / 4] = (1 << gpioPin); //GPCLR0
}

int gpioLevel0(volatile uint32_t *gpio, int gpioPin){
    return (gpio[0x34 / 4] & (1 << gpioPin)) != 0; //GPLEV0
}