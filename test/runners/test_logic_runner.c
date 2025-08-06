#include "unity.h"
#include "hw_mock.h" 
#include <unistd.h>       

void setUp(void) {
    reset_hw_mocks();
}

void tearDown(void) {
}

void test_vibrate_logic_success_path(void);
void test_vibrate_open_gpiomem_fail(void);
void test_vibrate_open_flocked_fail(void);
void test_vibrate_mmap_failure(void);
void test_vibrate_flock_failure(void);
void test_vibrate_zero_count(void);

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_vibrate_logic_success_path);
    RUN_TEST(test_vibrate_open_gpiomem_fail);
    RUN_TEST(test_vibrate_open_flocked_fail);
    RUN_TEST(test_vibrate_mmap_failure);
    RUN_TEST(test_vibrate_flock_failure);
    RUN_TEST(test_vibrate_zero_count);

    return UNITY_END();
}