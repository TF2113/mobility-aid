CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -O2 -Iinclude -Isrc/utils

# Targets and Sources
C_TARGET = builds/mobility_aid
VIB_TARGET = builds/vibrate

C_SRCS = src/prox_sensor.c src/utils/tick.c src/utils/gpio_functions.c src/utils/config_loader.c
VIB_SRCS = src/vibrate.c src/utils/gpio_functions.c

# C program
mobility_aid:
	$(CC) $(CFLAGS) -o $(C_TARGET) $(C_SRCS)

vibrate:
	$(CC) $(CFLAGS) -o $(VIB_TARGET) $(VIB_SRCS)

# Clean build files
clean:
	rm -f $(C_TARGET)
