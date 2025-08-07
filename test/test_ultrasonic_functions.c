#include "unity.h"
#include "ultra_sensor.h"

#define usleep(usec) (void)0

static uint32_t mock_tick_return_values[10];
static int mock_tick_call_count;
uint32_t getCurrentTick(void) {
    return mock_tick_return_values[mock_tick_call_count++];
}

static int mock_gpio_return_values[60];
static int mock_gpio_call_count;

int gpioLevel0(volatile uint32_t *gpio, int pin) {
    (void)gpio; (void)pin;
    return mock_gpio_return_values[mock_gpio_call_count++];
}

void gpioSet0(volatile uint32_t *gpio, int pin) { (void)gpio; (void)pin; }
void gpioClear0(volatile uint32_t *gpio, int pin) { (void)gpio; (void)pin; }

void gpioSetFunction(volatile uint32_t *gpio, int pin, int functionCode) {
    (void)gpio; (void)pin; (void)functionCode;
}

void setUp(void) {
    mock_tick_call_count = 0;
    mock_gpio_call_count = 0;
    for(int i=0; i<10; i++) mock_tick_return_values[i] = 0;
    for(int i=0; i<60; i++) mock_gpio_return_values[i] = 0;
}
void tearDown(void) {}

void test_duration_to_distance_calculates_correctly(void) {
    float distance = duration_to_distance(588);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 10.0f, distance);
}

void test_duration_to_distance_for_zero_duration(void) {
    float distance = duration_to_distance(0);
    TEST_ASSERT_EQUAL_FLOAT(0.0f, distance);
}

void test_ultrasonic_get_distance_cm_happy_path(void) {
    mock_gpio_return_values[0] = 0;
    mock_gpio_return_values[1] = 1;
    mock_gpio_return_values[2] = 1;
    mock_gpio_return_values[3] = 0;
    mock_tick_return_values[0] = 10000;
    mock_tick_return_values[1] = 10000 + 588;
    float distance = ultrasonic_get_distance_cm(NULL);
    TEST_ASSERT_FLOAT_WITHIN(0.01f, 10.0f, distance);
}

void test_ultrasonic_get_distance_cm_timeout_waiting_for_echo_high(void) {
    float distance = ultrasonic_get_distance_cm(NULL);
    TEST_ASSERT_EQUAL_FLOAT(-1.0f, distance);
}

void test_ultrasonic_get_distance_cm_timeout_waiting_for_echo_low(void) {
    mock_gpio_return_values[0] = 0;
    mock_gpio_return_values[1] = 1;
    for(int i=2; i<60; i++) mock_gpio_return_values[i] = 1;
    mock_tick_return_values[0] = 5000;

    float distance = ultrasonic_get_distance_cm(NULL);
    TEST_ASSERT_EQUAL_FLOAT(-2.0f, distance);
}