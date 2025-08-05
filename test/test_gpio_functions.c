#include <stdint.h>
#include "unity.h"
#include "gpio_functions.h" 

#define MOCK_GPIO_REGISTER_SIZE 40
static uint32_t mock_gpio_registers[MOCK_GPIO_REGISTER_SIZE];

#define GPFSEL0 (0x00 / 4)
#define GPFSEL1 (0x04 / 4)
#define GPSET0 (0x1c / 4)
#define GPCLR0 (0x28 / 4)
#define GPLEV0 (0x34 / 4)

void setUp(void) {
    for (int i = 0; i < MOCK_GPIO_REGISTER_SIZE; i++) {
        mock_gpio_registers[i] = 0;
    }
}

void tearDown(void) {
}

void test_gpioSetFunction_set_pin_output(void) {
    int pin = 17;
    int functionCode = 1;
    gpioSetFunction(mock_gpio_registers, pin, functionCode);
    // Pin 17 bits start at (17 % 10) * 3 = 21
    uint32_t expected_value = (1 << 21);
    TEST_ASSERT_EQUAL_UINT32(expected_value, mock_gpio_registers[GPFSEL1]);
}

void test_gpioSet0_correct_bit_register(void) {
    int pin = 22;
    gpioSet0(mock_gpio_registers, pin);
    uint32_t expected_value = (1 << pin);
    TEST_ASSERT_EQUAL_UINT32(expected_value, mock_gpio_registers[GPSET0]);
}

void test_gpioClear0_correct_bit_register(void) {
    int pin = 5;
    gpioClear0(mock_gpio_registers, pin);
    uint32_t expected_value = (1 << pin);
    TEST_ASSERT_EQUAL_UINT32(expected_value, mock_gpio_registers[GPCLR0]);
}

void test_gpioLevel0_return_1_when_set(void) {
    int pin = 18;
    mock_gpio_registers[GPLEV0] = (1 << pin);
    TEST_ASSERT_EQUAL(1, gpioLevel0(mock_gpio_registers, pin));
}

void test_gpioLevel0_return_0_when_clear(void) {
    int pin = 18;
    TEST_ASSERT_EQUAL(0, gpioLevel0(mock_gpio_registers, pin));
}

void test_gpioSetFunction_changes_single_pin(void) {
    uint32_t initialState = (2 << 6);
    mock_gpio_registers[GPFSEL0] = initialState;

    gpioSetFunction(mock_gpio_registers, 9, 0);
    TEST_ASSERT_EQUAL_UINT32(initialState, mock_gpio_registers[GPFSEL0]);

    gpioSetFunction(mock_gpio_registers, 5, 1);
    uint32_t expected_state = initialState | (1 << 15);
    TEST_ASSERT_EQUAL_UINT32(expected_state, mock_gpio_registers[GPFSEL0]);
}

void test_gpioSetFunction_handles_register_boundary(void) {
    int pin_in_reg0 = 9;
    int pin_in_reg1 = 10;
    gpioSetFunction(mock_gpio_registers, pin_in_reg0, 1);
    gpioSetFunction(mock_gpio_registers, pin_in_reg1, 2);

    uint32_t expected_reg0 = (1 << 27);
    TEST_ASSERT_EQUAL_UINT32(expected_reg0, mock_gpio_registers[GPFSEL0]);

    uint32_t expected_reg1 = (2 << 0);
    TEST_ASSERT_EQUAL_UINT32(expected_reg1, mock_gpio_registers[GPFSEL1]);
}

void test_gpioClear0_clears_correct_bit_multiple(void) {
    uint32_t initial_levels = (1 << 5) | (1 << 12) | (1 << 30);
    mock_gpio_registers[GPLEV0] = initial_levels;

    gpioClear0(mock_gpio_registers, 12);
    mock_gpio_registers[GPLEV0] &= ~(1 << 12);

    TEST_ASSERT_EQUAL(1, gpioLevel0(mock_gpio_registers, 5));  
    TEST_ASSERT_EQUAL(0, gpioLevel0(mock_gpio_registers, 12)); 
    TEST_ASSERT_EQUAL(1, gpioLevel0(mock_gpio_registers, 30)); 
}

void test_gpioLevel0_reads_correct_multiple_high(void) {
    mock_gpio_registers[GPLEV0] = (1 << 5) | (1 << 12) | (1 << 30);
    TEST_ASSERT_EQUAL(1, gpioLevel0(mock_gpio_registers, 12));
    TEST_ASSERT_EQUAL(0, gpioLevel0(mock_gpio_registers, 10));
    TEST_ASSERT_EQUAL(1, gpioLevel0(mock_gpio_registers, 30));
}