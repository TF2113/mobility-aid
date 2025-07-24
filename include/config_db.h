#ifndef CONFIG_DB_H
#define CONFIG_DB_H

#include <stdint.h>
#include <sqlite3.h>

extern int prox_vibrate;   
extern int prox_yellow_led;

int get_int_config(sqlite3 *db, const char *key, int default_val);

#endif