#include <stdio.h> //Standard library for input/output, used for perror()
#include <stdint.h> //Used for uint32_t, key for memory management
#include <unistd.h> //Low-level system calls like close() and usleep()
#include <fcntl.h> //Used for open() on /dev/gpiomem
#include <sys/mman.h> //Memory management for mmap() and munmap()

#define GPIO_OFFSET 0x0 //Storing /dev/gpiomem in virtual memory via mmap()
#define MEM_BLOCK 4096 //4KB memory block for storing register data

//RPI 4 Model B GPIO register addresses 
//https://datasheets.raspberrypi.com/bcm2711/bcm2711-peripherals.pdf

#define GPSEL1 0x04 //Function select for GPIO pins 10-19 (LED)
#define GPSEL2 0x08 //Function select for GPIO pins 20-29 (TRIG, ECHO)
#define GPSET0 0x1c //Set GPIO pin output
#define GPCLR0 0x28 //Clear GPIO pin output
#define GPLEV0 0x34 //Read GPIO level

//Open GPIO memory register file

int main(){

    int fd = open("/dev/gpiomem", O_RDWR | O_SYNC); //READ & WRITE perms and SYNC to prevent program from continuing before writes are finished

    if (fd < 0){
        perror("Access to gpiomem failed\n");
        return 1;
    }

    //32 bit pointer to gpiomem for manipulating GPIO register via mmap()
    volatile uint32_t *gpio = mmap(NULL,                    //Kernel chooses virtual memory address
                                   MEM_BLOCK,               //Size of data to be mapped
                                   PROT_READ | PROT_READ,   //Allows data to be read and written to
                                   MAP_SHARED,              //Other processes can use data
                                   fd,                      //File descriptor (gpiomem)
                                   GPIO_OFFSET);            //Start of GPIO address within gpiomem (0x0)
                                   
    if (gpio == MAP_FAILED){
        perror("Failed to map gpio registers\n");
        close(fd);
        return 1;
    }
}