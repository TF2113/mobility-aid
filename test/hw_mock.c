#include "hw_mock.h"
#include "gpio_functions.h"
#include <sys/file.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

static const LOG_SIZE = 100;

static MockFailType s_fail_type;
static GpioCall s_gpio_call_log[LOG_SIZE];
static int s_gpio_call_count;
static long long s_total_usleep;
static uint32_t s_gpiomem_pointer;

void reset_hw_mocks(void) {
    s_fail_type = MOCK_SUCCESS;
    s_gpio_call_count = 0;
    s_total_usleep = 0;
    for (size_t i = 0; i < LOG_SIZE; i++){
        s_gpio_call_log[i] = (GpioCall){0};
    }   
}

void set_hw_mock_failure_mode(MockFailType type){
    s_fail_type = type;
}

int get_gpio_call_count(void) { 
    return s_gpio_call_count; 
}
const GpioCall* get_gpio_call(int index) { 
    return &s_gpio_call_log[index]; 
}
long long get_total_usleep_time(void) { 
    return s_total_usleep; 
}