#include "sys_logic.h"
#include "ultra_sensor.h"
#include "feedback.h"
#include "gpio_functions.h"
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>

// Global flag to control the main loop, modified by the signal handler
volatile bool running = true;
void handle_signal(int sig) {
    (void)sig; // Suppress unused parameter warning
    printf("\nSignal received, shutting down...\n");
    running = false;
}

int main() {
    AppState state = {0}; // Zero-initialize the entire state struct

    // Initialize all systems
    if (system_init(&state) != 0) {
        fprintf(stderr, "Fatal: System initialization failed.\n");
        return 1;
    }

    // Register signal handlers for clean shutdown
    signal(SIGINT, handle_signal);  // Catch Ctrl+C
    signal(SIGTERM, handle_signal); // Catch `kill` command

    time_t last_config_check = 0;
    const int CONFIG_CHECK_INTERVAL_S = 1;

    printf("System running. Press Ctrl+C to exit.\n");
    
    // Main application loop
    while (running) {
        // Always on: Green LED indicates the main loop is active
        gpioSet0(state.gpio, GREEN_LED);

        // Periodically check for and clean up terminated child processes
        system_reap_child_processes();

        // Periodically reload configuration from the database
        if (time(NULL) - last_config_check >= CONFIG_CHECK_INTERVAL_S) {
            system_load_config(&state);
            last_config_check = time(NULL);
        }

        // 1. Get Data from Sensor
        float distance = ultrasonic_get_distance_cm(state.gpio);

        // 2. Process Data and Provide Feedback
        if (distance > 2.0 && distance < 200.0) { // Filter out invalid readings
            printf("Distance: %.2f cm\n", distance);
            feedback_update(&state, distance);
        } else {
            // If distance is invalid, ensure feedback is off
            gpioClear0(state.gpio, RED_LED);
            gpioClear0(state.gpio, YELLOW_LED);
        }
        
        usleep(100000); // Main loop delay to prevent busy-looping
    }

    // Cleanly shut down all systems
    system_shutdown(&state);
    return 0;
}