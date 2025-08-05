#include <stdio.h>     
#include <stdlib.h>     
#include <sqlite3.h>    
#include "config_db.h"
#include "unity.h"

static sqlite3 *db;

void setUp(void) {
    if (sqlite3_open(":memory:", &db) != SQLITE_OK) {
        TEST_FAIL_MESSAGE("Failed to open in-memory database.");
    }

    char *err_msg = 0;
    const char *create_sql = "CREATE TABLE config (key TEXT PRIMARY KEY, value TEXT);";
    if (sqlite3_exec(db, create_sql, 0, 0, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        TEST_FAIL_MESSAGE("Failed to create 'config' table.");
    }
}

void tearDown(void) {
    sqlite3_close(db);
}

void insert_config(const char *key, const char *value) {
    char sql[256];
    snprintf(sql, sizeof(sql), "INSERT INTO config (key, value) VALUES ('%s', '%s');", key, value);
    
    char *err_msg = 0;
    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        sqlite3_free(err_msg);
        TEST_FAIL_MESSAGE("Failed to insert test data.");
    }
}

void test_get_int_config_key_found(void) {
    insert_config("prox_vibrate", "20");
    int result = get_int_config(db, "prox_vibrate", 99);
    TEST_ASSERT_EQUAL_INT(20, result);
}

void test_get_int_config_key_not_found(void) {
    int result = get_int_config(db, "non_existent_key", 150);
    TEST_ASSERT_EQUAL_INT(150, result);
}

void test_get_int_config_invalid_value(void) {
    insert_config("prox_yellow_led", "number");
    int result = get_int_config(db, "prox_yellow_led", 99);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_get_int_config_negative_value(void) {
    insert_config("neg_val", "-50");
    int result = get_int_config(db, "neg_val", 99);
    TEST_ASSERT_EQUAL_INT(-50, result);
}