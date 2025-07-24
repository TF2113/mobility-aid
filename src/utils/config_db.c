#include <stdio.h>     
#include <stdlib.h>     
#include <sqlite3.h>    
#include "config_db.h"

int prox_vibrate = 15;    
int prox_yellow_led = 25;

int get_int_config(sqlite3 *db, const char *key, int default_val) {
    sqlite3_stmt *stmt;
    int val = default_val;

    const char *sql = "SELECT value FROM config WHERE key = ?;";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to prepare statement\n");
        return default_val;
    }

    if (sqlite3_bind_text(stmt, 1, key, -1, SQLITE_STATIC) != SQLITE_OK) {
        fprintf(stderr, "Failed to bind key\n");
        sqlite3_finalize(stmt);
        return default_val;
    }

    int rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        const unsigned char *text_val = sqlite3_column_text(stmt, 0);
        if (text_val != NULL) {
            val = atoi((const char *)text_val);
        }
    }
    sqlite3_finalize(stmt);
    return val;
}
