#include "unity.h"
#include "vibrate_mock.h" 
#include <unistd.h>       

void setUp(void) {
    reset_vibrate_mock();
}

void tearDown(void) {
    if(access("stderr_test.txt", F_OK) == 0){
        remove("stderr_test.txt");
    }
}

void test_cli_vibrate_valid_args(void);
void test_cli_vibrate_invalid_num_args(void);
void test_cli_vibrate_negative_args(void);
void test_cli_vibrate_non_integer_count_arg(void);
void test_cli_vibrate_non_double_duration_arg(void);
void test_cli_vibrate_non_double_delay_arg(void);

int main(void) {

    UNITY_BEGIN();

    RUN_TEST(test_cli_vibrate_valid_args);
    RUN_TEST(test_cli_vibrate_invalid_num_args);
    RUN_TEST(test_cli_vibrate_negative_args);
    RUN_TEST(test_cli_vibrate_non_integer_count_arg);
    RUN_TEST(test_cli_vibrate_non_double_duration_arg);
    RUN_TEST(test_cli_vibrate_non_double_delay_arg);


    return UNITY_END();
}