#include "tick.h"
#include <sys/time.h>
#include <stdint.h>

//Returns current tick in microseconds
uint32_t getCurrentTick(void){
    struct timeval timeValue;
    gettimeofday(&timeValue, NULL);

    //Combine seconds and microseconds to a 64bit value to avoid multiplication overflow 
    uint64_t microseconds = (uint64_t)timeValue.tv_sec * 1000000 + timeValue.tv_usec;

    //Cast back to 32 bit and mask high 32 bits (Effectively 0x00000000FFFFFFFF)
    return (uint32_t)(microseconds & 0xFFFFFFFF);
}