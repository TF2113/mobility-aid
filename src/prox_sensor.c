#include <stdio.h> //Standard library for input/output, used for perror()
#include <stdint.h> //Used for uint32_t, key for memory management
#include <unistd.h> //Low-level system calls like close() and usleep()
#include <fcntl.h> //Used for open() on /dev/gpiomem
#include <sys/mman.h> //Memory management for mmap() and munmap()
#include "tick.h" //Used for getCurrentTick() defined in /utils/tick.c

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
                                   PROT_READ | PROT_WRITE,   //Allows data to be read and written to
                                   MAP_SHARED,              //Other processes can use data
                                   fd,                      //File descriptor (gpiomem)
                                   GPIO_OFFSET);            //Start of GPIO address within gpiomem (0x0)
                                   
    if (gpio == MAP_FAILED){
        perror("Failed to map gpio registers\n");
        close(fd);
        return 1;
    }

    uint32_t trig_reg = gpio[GPSEL2 / 4]; //Access GPSEL2 register

    //Sensor intialise, clear GPIO register and set to output 
    trig_reg &= ~(7 << 9); //Clear 3 bit register assigned to GPIO 23, physical pin 16, TRIG function on HC-SR04
    trig_reg |= (1 << 9); //Assign TRIG as output by setting 3 bit register to 001
    gpio[GPSEL2 / 4] = trig_reg; //Update GPIO register

    //Repeat process for ECHO register but setting as input
    uint32_t echo_reg = gpio[GPSEL2 / 4]; //Access GPSEL2 register

    echo_reg &= ~(7 << 12); //Clear 3 bit register assigned to GPIO 24, physical pin 18, ECHO function on HC-SR04
    gpio[GPSEL2 / 4] = echo_reg; //Update GPIO register

    //LED
    uint32_t led_reg = gpio[GPSEL1 / 4]; //Use GPSEL1 for GPIO 10-19 (LED = 17)

    led_reg &= ~(7 << 21);
    led_reg |= (1 << 21);
    gpio[GPSEL1 / 4] = led_reg;
    
    //Sensor Initialise
    gpio[GPSET0 / 4] = (1 << 23);
    usleep(500000);

    uint32_t startTick, endTick;

    for(int i = 0; i < 10; i++){
        
        gpio[GPSET0 / 4] = (1 << 23); //Set TRIG to HIGH
        usleep(20);                   //Short pulse
        gpio[GPCLR0 / 4] = (1 << 23); //Clear TRIG bits
        
        while((gpio[GPLEV0 / 4] & (1 << 24)) == 0); //Wait for ECHO level to change

        startTick = getCurrentTick(); //Get tick at change

        while((gpio[GPLEV0 / 4] & (1 << 24)) != 0); //Once change is detected

        endTick = getCurrentTick();                 //Get tick after ECHO is received

        uint32_t duration = endTick - startTick;    //Formula for calculating distance
        float distance_cm = duration / 58.8;        //58.8 (rounded up) is time taken (µs) for sound to travel 1cm round trip at 20c

        if (distance_cm < 5){
            gpio[GPSET0 / 4] = (1 << 17);           //Flash LED when within 5cm proximity to the sensor
            usleep(500000);
            gpio[GPCLR0 / 4] = (1 << 17);
        }

        printf("Measurement %d\n", i+1);

        usleep(1000000);
    }

    munmap((void *)gpio, MEM_BLOCK);   //Unmap memory
    close(fd);                         //Close /dev/gpiomem file

    return 0;
}