#include "unity.h"
#include "hw_mock.h" 
#include "vibrate_mock.h" 
#include <unistd.h>       

void setUp(void) {
    reset_hw_mocks();
    reset_vibrate_mock();
}

void tearDown(void) {
    if(access("stderr_test.txt", F_OK) == 0){
        remove("stderr_test.txt");
    }
}

void test_cli_vibrate_valid_args(void);
void test_cli_vibrate_invalid_num_args(void);

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_cli_vibrate_valid_args);
    RUN_TEST(test_cli_vibrate_invalid_num_args);

    return UNITY_END();
}