#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "gpio_functions.h"

#define GPIO_OFFSET 0x0 // Storing /dev/gpiomem in virtual memory via mmap()
#define MEM_BLOCK 4096  // 4KB memory block for storing register data

#define VIB_MOTOR 13

int main()
{

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

    if (gpio == MAP_FAILED)
    {
        perror("Failed to map gpio registers\n");
        close(fd);
        return 1;
    }

    gpioSetFunction(gpio, VIB_MOTOR, 0b001);

    for (int i = 0; i < 3; i++)
    {
        gpioSet0(gpio, VIB_MOTOR);
        usleep(350000);
        gpioClear0(gpio, VIB_MOTOR);
        usleep(250000);
    }

    munmap((void *)gpio, MEM_BLOCK); // Unmap memory
    close(fd);                       // Close /dev/gpiomem file

    return 0;
}