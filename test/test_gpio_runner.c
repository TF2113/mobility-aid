#include "unity.h"
#include "gpio_functions.h"   

void test_gpioSetFunction_set_pin_output(void);
void test_gpioSet0_correct_bit_register(void);
void test_gpioClear0_correct_bit_register(void);
void test_gpioLevel0_return_1_when_set(void);
void test_gpioLevel0_return_0_when_clear(void);
void test_gpioSetFunction_changes_single_pin(void);
void test_gpioSetFunction_handles_register_boundary(void);
void test_gpioClear0_clears_correct_bit_multiple(void);
void test_gpioLevel0_reads_correct_multiple_high(void);

int main(void) {

    UNITY_BEGIN();
 
    RUN_TEST(test_gpioSetFunction_set_pin_output);
    RUN_TEST(test_gpioSet0_correct_bit_register);
    RUN_TEST(test_gpioClear0_correct_bit_register);
    RUN_TEST(test_gpioLevel0_return_1_when_set);
    RUN_TEST(test_gpioLevel0_return_0_when_clear);
    RUN_TEST(test_gpioSetFunction_changes_single_pin);
    RUN_TEST(test_gpioSetFunction_handles_register_boundary);
    RUN_TEST(test_gpioClear0_clears_correct_bit_multiple);
    RUN_TEST(test_gpioLevel0_reads_correct_multiple_high);

    return UNITY_END();
}