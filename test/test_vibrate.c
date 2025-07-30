#include "unity.h"
#include "vibrate.h"
#include <stdio.h>
#include <string.h>

void setUp(void) {

}

void tearDown(void) {

}

void test_cli_vibrate_valid_num_args(void){
    
}

void test_cli_vibrate_invalid_num_args(void){
    FILE* orig_stderr = stderr;
    stderr = freopen("stderr_test.txt", "w", stderr);
    if (stderr == NULL) {
        TEST_FAIL_MESSAGE("Failed to redirect stderr");
    }

    // Prepare invalid arguments
    char *argv[] = {"test_program", "another_arg"};
    int argc = 2;

    // Call the function with invalid arguments
    int result = cli_vibrate(argc, argv);

    // Restore stderr
    fclose(stderr);
    stderr = orig_stderr;

    // Read the error message from the temporary file
    char err_buffer[256];
    FILE* test_output = fopen("stderr_test.txt", "r");
    if (test_output == NULL) {
        TEST_FAIL_MESSAGE("Failed to open stderr output file for reading");
    }
    fgets(err_buffer, sizeof(err_buffer), test_output);
    fclose(test_output);
    remove("stderr_test.txt");

    // Construct the expected error message
    char expected_error[256];
    sprintf(expected_error, "Usage: %s <vibration_count> <duration(s)> <delay(s)>\n", argv[0]);

    // Assert that the function returned an error and printed the correct message
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_STRING(expected_error, err_buffer);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_cli_vibrate_invalid_num_args);
    return UNITY_END();
}