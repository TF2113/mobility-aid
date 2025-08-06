#include "vibrate.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/file.h>
#include "gpio_functions.h"
#include <errno.h>

#define GPIO_OFFSET 0x0
#define MEM_BLOCK 4096
#define VIB_MOTOR 13
#define LOCK_FILE_PATH "./builds/tmp/vibrate.lock"

static void perform_vibration_cycle(volatile uint32_t *gpio, int count, int duration_us, int delay_us) {
    // Set the pin function once before starting the cycle
    gpioSetFunction(gpio, VIB_MOTOR, 1); // 1 = Output

    for (int i = 0; i < count; i++) {
        gpioSet0(gpio, VIB_MOTOR);
        usleep(duration_us);
        gpioClear0(gpio, VIB_MOTOR);
        usleep(delay_us);
    }
}

int vibrate(int count, double duration_s, double delay_s) {
    // Acquire a lock first to prevent multiple instances from using the hardware.
    int lock_fd = open(LOCK_FILE_PATH, O_CREAT | O_RDWR, 0666);
    if (lock_fd < 0) {
        perror("Cannot open lock file");
        return 1;
    }

    // Try to get an exclusive, non-blocking lock.
    if (flock(lock_fd, LOCK_EX | LOCK_NB) < 0) {
        if (errno == EWOULDBLOCK) {
            // This is not an error. Another process is already vibrating.
            fprintf(stderr, "Vibrator already active. Exiting.\n");
            close(lock_fd);
            return 0; // Exit successfully
        }
        perror("flock failed");
        close(lock_fd);
        return 1;
    }

    int gpio_fd = open("/dev/gpiomem", O_RDWR | O_SYNC);
    if (gpio_fd < 0) {
        perror("Access to gpiomem failed");
        flock(lock_fd, LOCK_UN); // Release lock on failure
        close(lock_fd);
        return 1;
    }

    volatile uint32_t *gpio = mmap(NULL, MEM_BLOCK, PROT_READ | PROT_WRITE, MAP_SHARED, gpio_fd, GPIO_OFFSET);
    if (gpio == MAP_FAILED) {
        perror("Failed to map gpio registers");
        close(gpio_fd);
        flock(lock_fd, LOCK_UN);
        close(lock_fd);
        return 1;
    }

    perform_vibration_cycle(gpio, count, (int)(duration_s * 1000000), (int)(delay_s * 1000000));

    munmap((void *)gpio, MEM_BLOCK);
    close(gpio_fd);
    flock(lock_fd, LOCK_UN);
    close(lock_fd);
    
    return 0;
}

int cli_vibrate(int argc, char *argv[]){
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <vibration_count> <duration(s)> <delay(s)>\n", argv[0]);
        return 1;
    }
    
    char *endptr_count;
    char *endptr_duration;
    char *endptr_delay;
    long count_long;
    double duration, delay;

    errno = 0;

    count_long = strtol(argv[1], &endptr_count, 10);

    if (errno != 0 || endptr_count == argv[1] || *endptr_count != '\0') {
        fprintf(stderr, "Error: 'vibration_count' must be a valid integer.\n");
        return 1;
    }

    duration = strtod(argv[2], &endptr_duration);
    if (endptr_duration == argv[2] || *endptr_duration != '\0') {
        fprintf(stderr, "Error: 'duration' must be a valid number.\n");
        return 1;
    }

    delay = strtod(argv[3], &endptr_delay);
    if (endptr_delay == argv[3] || *endptr_delay != '\0') {
        fprintf(stderr, "Error: 'delay' must be a valid number.\n");
        return 1;
    }

    if (count_long <= 0 || duration < 0 || delay < 0) {
        fprintf(stderr, "Error: count must be positive, duration and delay cannot be negative.\n");
        return 1;
    }

    int count = (int)count_long;

    return vibrate(count, duration, delay);
}

int main(int argc, char *argv[]){
    return cli_vibrate(argc, argv);
}
