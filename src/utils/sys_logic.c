#include "sys_logic.h"
#include "config_db.h"
#include "gpio_functions.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

int system_init(AppState *state, const char *db_path) {
    printf("Initializing system...\n");

    // Open and validate SQLite database connection
    if (sqlite3_open(db_path, &state->db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open DB: %s\n", sqlite3_errmsg(state->db));
        return -1;
    }

    // Open GPIO memory file descriptor
    state->gpio_fd = open("/dev/gpiomem", O_RDWR | O_SYNC);
    if (state->gpio_fd < 0) {
        perror("Access to gpiomem failed");
        sqlite3_close(state->db); // Clean up DB connection on failure
        return -1;
    }

    // Map GPIO registers into virtual memory
    state->gpio = mmap(NULL, MEM_BLOCK, PROT_READ | PROT_WRITE, MAP_SHARED, state->gpio_fd, GPIO_OFFSET);
    if (state->gpio == MAP_FAILED) {
        perror("Failed to map gpio registers");
        close(state->gpio_fd);
        sqlite3_close(state->db);
        return -1;
    }

    // Set GPIO function modes
    gpioSetFunction(state->gpio, VIB_MOTOR, 1);
    gpioSetFunction(state->gpio, RED_LED, 1);
    gpioSetFunction(state->gpio, GREEN_LED, 1);
    gpioSetFunction(state->gpio, YELLOW_LED, 1);
    gpioSetFunction(state->gpio, TRIG, 1);
    gpioSetFunction(state->gpio, ECHO, 0);

    // Load initial configuration
    system_load_config(state);
    
    // Sensor Initialise
    gpioClear0(state->gpio, TRIG);
    usleep(500000); // Allow sensor to settle

    printf("System initialized successfully.\n");
    return 0;
}

void system_shutdown(AppState *state) {
    printf("Shutting down cleanly...\n");
    
    // Ensure all outputs are off
    gpioClear0(state->gpio, GREEN_LED);
    gpioClear0(state->gpio, YELLOW_LED);
    gpioClear0(state->gpio, RED_LED);
    gpioClear0(state->gpio, VIB_MOTOR);
    
    // Unmap memory and close files
    munmap((void *)state->gpio, MEM_BLOCK);
    close(state->gpio_fd);
    sqlite3_close(state->db);
}

void system_load_config(AppState *state) {
    // Default values are hardcoded here as a fallback
    state->prox_vibrate = get_int_config(state->db, "prox_vibrate", 15);
    state->prox_yellow_led = get_int_config(state->db, "prox_yellow_led", 25);
    printf("[CONFIG] Loaded prox_vibrate=%d, prox_yellow_led=%d\n", state->prox_vibrate, state->prox_yellow_led);
}

void system_reap_child_processes(void) {
    while (waitpid(-1, NULL, WNOHANG) > 0) {
    }
}