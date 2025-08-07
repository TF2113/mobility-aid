#include "unity.h"
#include "ultra_sensor.h"

void test_duration_to_distance_calculates_correctly(void);
void test_duration_to_distance_for_zero_duration(void);
void test_ultrasonic_get_distance_cm_happy_path(void);
void test_ultrasonic_get_distance_cm_timeout_waiting_for_echo_high(void);
void test_ultrasonic_get_distance_cm_timeout_waiting_for_echo_low(void);

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_duration_to_distance_calculates_correctly);
    RUN_TEST(test_duration_to_distance_for_zero_duration);
    RUN_TEST(test_ultrasonic_get_distance_cm_happy_path);
    RUN_TEST(test_ultrasonic_get_distance_cm_timeout_waiting_for_echo_high);
    RUN_TEST(test_ultrasonic_get_distance_cm_timeout_waiting_for_echo_low);

    return UNITY_END();
}