#include <stdio.h>    //Standard library for input/output, used for perror()
#include <stdint.h>   //Used for uint32_t, key for memory management
#include <unistd.h>   //Low-level system calls like close() and usleep()
#include <fcntl.h>    //Used for open() on /dev/gpiomem
#include <sys/mman.h> //Memory management for mmap() and munmap()
#include "tick.h"     //Used for getCurrentTick() defined in /utils/tick.c
#include "gpio_functions.h" //Used for GPIO functions defined in /utils/gpio_functions.h
#include "config_loader.h" //Used for loading config files

#define GPIO_OFFSET 0x0 // Storing /dev/gpiomem in virtual memory via mmap()
#define MEM_BLOCK 4096  // 4KB memory block for storing register data

//GPIO declarations
#define VIB_MOTOR 13
#define RED_LED 17
#define TRIG 23
#define ECHO 24
#define YELLOW_LED 22
#define GREEN_LED 27

void load_config(const char *filename);

// Open GPIO memory register file
int main() {

    load_config("./src/configs/config.txt");

    int fd = open("/dev/gpiomem", O_RDWR | O_SYNC); // READ & WRITE perms and SYNC to prevent program from continuing before writes are finished

    if (fd < 0) {
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

    if (gpio == MAP_FAILED) {
        perror("Failed to map gpio registers\n");
        close(fd);
        return 1;
    }

    //Set GPIO to output
    gpioSetFunction(gpio, VIB_MOTOR, 0b001);
    gpioSetFunction(gpio, RED_LED, 0b001);
    gpioSetFunction(gpio, GREEN_LED, 0b001);
    gpioSetFunction(gpio, YELLOW_LED, 0b001);
    gpioSetFunction(gpio, TRIG, 0b001);

    //Set ECHO to input
    gpioSetFunction(gpio, ECHO, 0b000);

    uint32_t startTick, endTick;
    
    // Sensor Initialise
    gpioClear0(gpio, TRIG);
    usleep(500000); // Allow sensor to settle

    for (int i = 0; i < 100; i++) {

        gpioSet0(gpio, GREEN_LED); // Turn on Green LED while program is active

        gpioSet0(gpio, TRIG); // Set TRIG to HIGH
        usleep(20); // Short pulse
        gpioClear0(gpio, TRIG); // Clear TRIG bits

        while (gpioLevel0(gpio, ECHO) == 0); // Wait for ECHO level to change
        startTick = getCurrentTick(); // Get tick at change

        while (gpioLevel0(gpio, ECHO) != 0); // Wait until ECHO level resets to 0
        endTick = getCurrentTick(); // Get tick after ECHO is received

        uint32_t duration = endTick - startTick; // Formula for calculating distance
        float distance_cm = duration / 58.8;     // 58.8 (rounded up) is time taken (µs) for sound to travel 1cm at 20c

        int numBlink = prox_vibrate / distance_cm;
        int delay = 40000 * distance_cm;

        if (distance_cm < prox_vibrate) {
            for (int j = 0; j < numBlink; j++) {
                gpioSet0(gpio, RED_LED); // Flash LED and vibrate motor when within 15cm proximity to the sensor corresponding to the distance, quickens as the distance decreases
                gpioSet0(gpio, VIB_MOTOR);
                usleep(delay);
                gpioClear0(gpio, VIB_MOTOR);
                gpioClear0(gpio, RED_LED);
                usleep(delay);
            }
        }

        if (distance_cm < prox_yellow_led){
            gpioSet0(gpio, YELLOW_LED);
        } else {
            gpioClear0(gpio, YELLOW_LED);
        }

        printf("Measurement %d\nDistance: %.2f cm\n\n", i + 1, distance_cm);
        usleep(100000);
    }

    gpioClear0(gpio, GREEN_LED); // Turn off green LED
    gpioClear0(gpio, YELLOW_LED); //Turn off yellow LED in case last reading <20cm
    munmap((void *)gpio, MEM_BLOCK); // Unmap memory
    close(fd);                       // Close /dev/gpiomem file

    return 0;
}