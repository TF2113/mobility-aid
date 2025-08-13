#include <stdint.h>

static uint32_t fake_tick_counter = 1000000;

uint32_t getCurrentTick(void) {
    fake_tick_counter += 588; // Distance of 10cm every time
    return fake_tick_counter;
}