#include "vibrate.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/file.h>
#include "gpio_functions.h"
#include <errno.h>

#define GPIO_OFFSET 0x0 // Storing /dev/gpiomem in virtual memory via mmap()
#define MEM_BLOCK 4096  // 4KB memory block for storing register data

#define VIB_MOTOR 13

int vibrate(int count, double duration, double delay){

    int fd = open("/dev/gpiomem", O_RDWR | O_SYNC); // READ & WRITE perms and SYNC to prevent program from continuing before writes are finished

    if (fd < 0)
    {
        perror("Access to gpiomem failed\n");
        return 1;
    }

    // 32 bit pointer to gpiomem for manipulating GPIO register via mmap()
    volatile uint32_t *gpio = mmap(NULL,                   // Kernel chooses virtual memory address
                                   MEM_BLOCK,              // Size of data to be mapped
                                   PROT_READ | PROT_WRITE, // Allows data to be read and written to
                                   MAP_SHARED,             // Other processes can use data
                                   fd,                     // File descriptor (gpiomem)
                                   GPIO_OFFSET);           // Start of GPIO address within gpiomem (0x0)

    if (gpio == MAP_FAILED){
        perror("Failed to map gpio registers\n");
        close(fd);
        return 1;
    }

    int lock_fd = open("./builds/tmp/vibrate.lock", O_CREAT | O_RDWR, 0666);
    if (lock_fd < 0) {
        perror("open lock file");
        return 1;
    }
    if (flock(lock_fd, LOCK_EX | LOCK_NB) < 0) {
        close(lock_fd);
        return 0;
    }

    gpioSetFunction(gpio, VIB_MOTOR, 0b001);

    for (int i = 0; i < count; i++){
        gpioSet0(gpio, VIB_MOTOR);
        usleep((int)(duration * 1000000));
        gpioClear0(gpio, VIB_MOTOR);
        usleep((int)(delay * 1000000));
    }

    munmap((void *)gpio, MEM_BLOCK); // Unmap memory
    close(fd);                       // Close /dev/gpiomem file
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