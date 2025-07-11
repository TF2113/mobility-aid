CC = gcc

# Compiler Flags
CFLAGS = -Wall -Wextra -O2 -Iinclude -Isrc/utils

# Targets and Sources
C_TARGET = builds/mobility-aid

C_SRCS = src/prox_sensor.c src/utils/tick.c src/utils/gpio_functions.c

# C program
mobility-aid:
	$(CC) $(CFLAGS) -o $(C_TARGET) $(C_SRCS)

# Clean build files
clean:
	rm -f $(C_TARGET) $(CAPTURE_TARGET) $(INFERENCE_TARGET)
