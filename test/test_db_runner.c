#include "unity.h"
#include <unistd.h>    
#include <sqlite3.h>    
#include "config_db.h"   

void test_get_int_config_key_found(void);
void test_get_int_config_key_not_found(void);
void test_get_int_config_table_not_found(void);
void test_get_int_config_invalid_value(void);
void test_get_int_config_negative_value(void);
void test_get_int_config_null_value(void);
void test_get_int_config_empty_value(void);
void test_get_int_config_valid_zero_value(void);

int main(void) {

    UNITY_BEGIN();

    RUN_TEST(test_get_int_config_key_found);
    RUN_TEST(test_get_int_config_key_not_found);
    RUN_TEST(test_get_int_config_table_not_found);
    RUN_TEST(test_get_int_config_invalid_value);
    RUN_TEST(test_get_int_config_negative_value);
    RUN_TEST(test_get_int_config_null_value);
    RUN_TEST(test_get_int_config_empty_value);
    RUN_TEST(test_get_int_config_valid_zero_value);
    
    return UNITY_END();
}