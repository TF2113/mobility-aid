#include "unity.h"
#include "hw_mock.h" 
#include "vibrate.h"  

int vibrate(int count, double duration, double delay);

void test_vibrate_logic_success_path(void) {
    int count = 2;
    double duration = 0.1; 
    double delay = 0.2;    

    int result = vibrate(count, duration, delay);

    TEST_ASSERT_EQUAL_INT(0, result);
    
    TEST_ASSERT_EQUAL_INT(5, get_gpio_call_count()); // Expected: 1 for set-function + (2 loops * 2 calls-per-loop) = 5

    const GpioCall* call;

    call = get_gpio_call(0);
    TEST_ASSERT_EQUAL_INT(GPIO_SET_FUNC, call->type);
    TEST_ASSERT_EQUAL_INT(13, call->gpioPin);
    TEST_ASSERT_EQUAL_INT(0b001, call->funcCode); // Function code for output

    call = get_gpio_call(1);
    TEST_ASSERT_EQUAL_INT(GPIO_SET, call->type); // First vibration ON

    call = get_gpio_call(2);
    TEST_ASSERT_EQUAL_INT(GPIO_SET_CLEAR, call->type); // First vibration OFF

    call = get_gpio_call(3);
    TEST_ASSERT_EQUAL_INT(GPIO_SET, call->type); // Second vibration ON

    call = get_gpio_call(4);
    TEST_ASSERT_EQUAL_INT(GPIO_SET_CLEAR, call->type); // Second vibration OFF

    long long expected_usleep_us = (long long)(count * (duration + delay) * 1000000);
    TEST_ASSERT_EQUAL_UINT64(expected_usleep_us, get_total_usleep_time());
}

void test_vibrate_open_gpiomem_fail(void){
    set_hw_mock_failure_mode(MOCK_FAILED_OPEN_GPIOMEM);
    int result = vibrate(1, 0.1, 0.1);
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_vibrate_open_flocked_fail(void){
    set_hw_mock_failure_mode(MOCK_FAILED_OPEN_FLOCK);
    int result = vibrate(1, 0.1, 0.1);
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_vibrate_mmap_failure(void) {
    set_hw_mock_failure_mode(MOCK_FAILED_MMAP);
    int result = vibrate(1, 0.1, 0.1);
    TEST_ASSERT_EQUAL_INT(1, result);
}

void test_vibrate_flock_failure(void) {
    set_hw_mock_failure_mode(MOCK_FAILED_FLOCK);
    int result = vibrate(1, 0.1, 0.1);                        
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(0, get_gpio_call_count());
}

void test_vibrate_zero_count(void) {
    int result = vibrate(0, 0.1, 0.1);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(1, get_gpio_call_count()); 
    TEST_ASSERT_EQUAL_UINT64(0, get_total_usleep_time());
}