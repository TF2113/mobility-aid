CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -Isrc/utils
TARGET = builds/mobility-aid
SRCS = src/prox_sensor.c src/utils/tick.c

mobility-aid:
			$(CC) $(CFLAGS) -o $(TARGET) $(SRCS) 