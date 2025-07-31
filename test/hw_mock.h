#ifndef HW_MOCKS_H_
#define HW_MOCKS_H_
#include <stdint.h>

void reset_hw_mocks(void);

typedef enum {
    MOCK_SUCCESS,
    MOCK_FAILED_MMAP,
    MOCK_FAILED_OPEN_GPIOMEM,
    MOCK_FAILED_FLOCK,
    MOCK_FAILED_OPEN_FLOCK
} MockFailType;

void set_hw_mock_failure_mode(MockFailType mode);

typedef enum {
    GPIO_GET_LEVEL,
    GPIO_SET_FUNC,
    GPIO_SET_CLEAR,
    GPIO_SET
} GpioCallType;

typedef struct {
    GpioCallType type;
    int gpioPin;
    int funcCode;
} GpioCall;

int get_gpio_call_count(void);
const GpioCall* get_gpio_call(int index);
long long get_total_usleep_time(void);

#endif