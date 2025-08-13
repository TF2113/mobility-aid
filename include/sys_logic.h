#ifndef SYS_LOGIC_H
#define SYS_LOGIC_H

#include <stdint.h>
#include <stdbool.h>
#include <sqlite3.h>

// Define GPIO pins
#define VIB_MOTOR 13
#define RED_LED 17
#define TRIG 23
#define ECHO 24
#define YELLOW_LED 22
#define GREEN_LED 27

// Memory mapping constants
#define GPIO_OFFSET 0x0
#define MEM_BLOCK 4096

extern volatile bool running;

// Struct to hold application state
typedef struct {
    volatile uint32_t *gpio;
    sqlite3 *db;
    int gpio_fd;

    // Configuration values loaded from DB
    int prox_vibrate;
    int prox_yellow_led;

} AppState;

int system_init(AppState *state, const char *db_path);

void system_shutdown(AppState *state);

void system_load_config(AppState *state);

void system_reap_child_processes(void);

#endif