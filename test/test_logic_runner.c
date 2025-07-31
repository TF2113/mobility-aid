#include "unity.h"
#include "hw_mock.h" 
#include <unistd.h>       

void setUp(void) {
    reset_hw_mocks();

}

void tearDown(void) {

}
void test_vibrate_logic_success_path(void);


int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_vibrate_logic_success_path);

    return UNITY_END();
}