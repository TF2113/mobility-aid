#include "unity.h"
#include "vibrate_mock.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

static const char* STDERR_OUTPUT_FILE = "stderr_test.txt";

#define ASSERT_CLI_ERROR(argv_array, expected_error_string) \
    do { \
        FILE* orig_stderr = stderr; \
        stderr = freopen(STDERR_OUTPUT_FILE, "w", stderr); \
        if (stderr == NULL) { \
            TEST_FAIL_MESSAGE("Failed to redirect stderr"); \
        } \
        \
        /* The macro automatically calculates argc */ \
        int argc = sizeof(argv_array) / sizeof(argv_array[0]); \
        int result = cli_vibrate(argc, argv_array); \
        \
        fclose(stderr); \
        stderr = orig_stderr; \
        \
        char err_buffer[256] = {0}; \
        FILE* file = fopen(STDERR_OUTPUT_FILE, "r"); \
        if (file) { \
            fgets(err_buffer, sizeof(err_buffer), file); \
            fclose(file); \
        } \
        remove(STDERR_OUTPUT_FILE); \
        err_buffer[strcspn(err_buffer, "\n")] = 0; /* Trim newline */ \
        \
        TEST_ASSERT_EQUAL_INT(1, result); \
        TEST_ASSERT_EQUAL_STRING(expected_error_string, err_buffer); \
    } while (0)


int cli_vibrate(int argc, char *argv[]);

void test_cli_vibrate_valid_args(void){
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

void test_cli_vibrate_no_args(void){
    char *argv[] = {"test_program"};

    char expected_error[256];
    sprintf(expected_error, "Usage: %s <vibration_count> <duration(s)> <delay(s)>", argv[0]);
    ASSERT_CLI_ERROR(argv, expected_error);
}

void test_cli_vibrate_invalid_num_args(void){
    char *argv[] = {"test_program", "1"};

    char expected_error[256];
    sprintf(expected_error, "Usage: %s <vibration_count> <duration(s)> <delay(s)>", argv[0]);
    ASSERT_CLI_ERROR(argv, expected_error);
}

void test_cli_vibrate_negative_args(void){
    char *argv[] = {"./vibrate_test", "-5", "1.5", "0.5"};
    ASSERT_CLI_ERROR(argv, "Error: count must be positive, duration and delay cannot be negative.");
}

void test_cli_vibrate_non_integer_count_arg(void){
    char *argv[] = {"./vibrate_test", "five", "1.5", "0.5"};
    ASSERT_CLI_ERROR(argv, "Error: 'vibration_count' must be a valid integer.");
}

void test_cli_vibrate_non_double_duration_arg(void){
    char *argv[] = {"./vibrate_test", "5", "two", "0.5"};
    ASSERT_CLI_ERROR(argv, "Error: 'duration' must be a valid number.");
}

void test_cli_vibrate_non_double_delay_arg(void){
    char *argv[] = {"./vibrate_test", "5", "1.5", "one"};
    ASSERT_CLI_ERROR(argv, "Error: 'delay' must be a valid number.");
}