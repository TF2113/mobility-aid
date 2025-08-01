#include "unity.h"
#include "vibrate_mock.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

static const char* STDERR_OUTPUT_FILE = "stderr_test.txt";

int cli_vibrate(int argc, char *argv[]);

void test_cli_vibrate_valid_args(void){
    // Prepare valid arguments
    char *argv[] = {"./vibrate_test", "5", "1.5", "0.5"};
    int argc = 4;
    double range = 0.001;

    // Call the function with valid arguments
    int result = cli_vibrate(argc, argv);

    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_INT(1, get_vibrate_call_count());
    TEST_ASSERT_EQUAL_INT(5, get_last_vibration_count_arg());
    TEST_ASSERT_DOUBLE_WITHIN(range, 1.5, get_last_duration_arg());
    TEST_ASSERT_DOUBLE_WITHIN(range, 0.5, get_last_delay_arg());

}

void test_cli_vibrate_invalid_num_args(void){
    FILE* orig_stderr = stderr;
    stderr = freopen(STDERR_OUTPUT_FILE, "w", stderr);
    if (stderr == NULL) {
        TEST_FAIL_MESSAGE("Failed to redirect stderr");
    }

    // Prepare invalid arguments
    char *argv[] = {"test_program", "1"};
    int argc = 2;

    // Call the function with invalid arguments
    int result = cli_vibrate(argc, argv);

    // Restore stderr
    fclose(stderr);
    stderr = orig_stderr;

    // Read the error message from the temporary file
    char err_buffer[256];
    FILE* test_output = fopen(STDERR_OUTPUT_FILE, "r");
    if (test_output == NULL) {
        TEST_FAIL_MESSAGE("Failed to open stderr output file for reading");
    }
    fgets(err_buffer, sizeof(err_buffer), test_output);
    fclose(test_output);
    remove(STDERR_OUTPUT_FILE);

    // Construct the expected error message
    char expected_error[256];
    sprintf(expected_error, "Usage: %s <vibration_count> <duration(s)> <delay(s)>\n", argv[0]);

    // Assert that the function returned an error and printed the correct message
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_STRING(expected_error, err_buffer);
}

void test_cli_vibrate_negative_args(void){
    FILE* orig_stderr = stderr;
    stderr = freopen(STDERR_OUTPUT_FILE, "w", stderr);
    if (stderr == NULL) {
        TEST_FAIL_MESSAGE("Failed to redirect stderr");
    }

    // Prepare invalid arguments
    char *argv[] = {"./vibrate_test", "-5", "1.5", "0.5"};
    int argc = 4;

    // Call the function with invalid arguments
    int result = cli_vibrate(argc, argv);

    // Restore stderr
    fclose(stderr);
    stderr = orig_stderr;

    // Read the error message from the temporary file
    char err_buffer[256];
    FILE* test_output = fopen(STDERR_OUTPUT_FILE, "r");
    if (test_output == NULL) {
        TEST_FAIL_MESSAGE("Failed to open stderr output file for reading");
    }
    fgets(err_buffer, sizeof(err_buffer), test_output);
    fclose(test_output);
    remove(STDERR_OUTPUT_FILE);

    // Construct the expected error message
    char expected_error[256];
    sprintf(expected_error, "Error: count, duration and delay must be positive.\n");

    // Assert that the function returned an error and printed the correct message
    TEST_ASSERT_EQUAL_INT(1, result);
    TEST_ASSERT_EQUAL_STRING(expected_error, err_buffer);
}