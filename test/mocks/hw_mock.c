#include "hw_mock.h"
#include "gpio_functions.h"
#include <sys/file.h>
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define LOG_SIZE 100

static MockFailType s_fail_type;
static GpioCall s_gpio_call_log[LOG_SIZE];
static int s_gpio_call_count;
static long long s_total_usleep;
static uint32_t s_gpiomem_pointer;

void reset_hw_mocks(void) {
    s_fail_type = MOCK_SUCCESS;
    s_gpio_call_count = 0;
    s_total_usleep = 0;
    for (int i = 0; i < LOG_SIZE; i++){
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

static void log_gpio_call(GpioCallType type, int gpioPin, int funcCode){
    if (s_gpio_call_count < 100) {
        s_gpio_call_log[s_gpio_call_count].type = type;
        s_gpio_call_log[s_gpio_call_count].gpioPin = gpioPin;
        s_gpio_call_log[s_gpio_call_count].funcCode = funcCode;
        s_gpio_call_count++;
    }
}

int open(const char* pathname, int flags, ...){
    (void)flags;
    if(s_fail_type == MOCK_FAILED_OPEN_GPIOMEM && strcmp(pathname, "/dev/gpiomem") == 0){
        return -1;
    }

    if(s_fail_type == MOCK_FAILED_OPEN_FLOCK){
        return -1;
    }

    return 10;
}

void* mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset){
    (void)addr; (void)length; (void)prot; (void)flags; (void)fd; (void)offset;
    if(s_fail_type == MOCK_FAILED_MMAP){
        return MAP_FAILED;
    }

    return &s_gpiomem_pointer;
}

int flock(int fd, int operation){
    (void)fd; (void)operation;
    if(s_fail_type == MOCK_FAILED_FLOCK){
        return -1;
    }
    return 0;
}

int usleep(unsigned int usec) {
    s_total_usleep += usec;
    return 0;
}

int close(int fd) {
    (void)fd; 
    return 0; 
}

int munmap(void *addr, size_t length) {
    (void)addr; (void)length; 
    return 0; 
}

void gpioSetFunction(volatile uint32_t *gpio, int pin, int function) {
    (void)gpio;
    log_gpio_call(GPIO_SET_FUNC, pin, function);
}

void gpioSet0(volatile uint32_t *gpio, int pin) {
    (void)gpio;
    log_gpio_call(GPIO_SET, pin, 0);
}

void gpioClear0(volatile uint32_t *gpio, int pin) {
    (void)gpio;
    log_gpio_call(GPIO_SET_CLEAR, pin, 0);
}
