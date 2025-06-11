CC = gcc
CPP = g++

CFLAGS = -Wall -Wextra -Iinclude -Isrc/utils
CPPFLAGS = $(CFLAGS)

C_TARGET = builds/mobility-aid
CPP_TARGET = builds/capture_image

C_SRCS = src/prox_sensor.c src/utils/tick.c
CPP_SRCS = src/cam/capture_image.cpp

OPENCV_CFLAGS := $(shell pkg-config --cflags opencv4)
OPENCV_LIBS := $(shell pkg-config --libs opencv4)

mobility-aid:
			$(CC) $(CFLAGS) -o $(C_TARGET) $(C_SRCS)
			$(CPP) $(CPPFLAGS) $(OPENCV_CFLAGS) -o $(CPP_TARGET) $(CPP_SRCS) $(OPENCV_LIBS)