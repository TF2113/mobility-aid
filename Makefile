CC = gcc
CPP = g++

# Compiler Flags
CFLAGS = -Wall -Wextra -O2 -Iinclude -Isrc/utils
CPPFLAGS = $(CFLAGS)

# Targets and Sources
C_TARGET = builds/mobility-aid
CAPTURE_TARGET = builds/capture_image
INFERENCE_TARGET = builds/img_inference

C_SRCS = src/prox_sensor.c src/utils/tick.c src/utils/gpio_functions.c
CAPTURE_SRCS = src/cam/capture_image.cpp
INFERENCE_SRCS = src/cam/img_inference.cpp

OPENCV_CFLAGS := $(shell pkg-config --cflags opencv4)
OPENCV_LIBS := $(shell pkg-config --libs opencv4)
NCNN_CFLAGS := -I/usr/local/include/ncnn
NCNN_LIBS := -L/usr/local/lib/ncnn -lncnn

all: mobility-aid capture_image img_inference

# C program
mobility-aid:
	$(CC) $(CFLAGS) -o $(C_TARGET) $(C_SRCS)

# C++ Programs
capture_image:
	$(CPP) $(CPPFLAGS) $(OPENCV_CFLAGS) $(NCNN_CFLAGS) -o $(CAPTURE_TARGET) $(CAPTURE_SRCS) \
	$(OPENCV_LIBS) $(NCNN_LIBS)

img_inference:
	$(CPP) $(CPPFLAGS) $(OPENCV_CFLAGS) $(NCNN_CFLAGS) -fopenmp -o $(INFERENCE_TARGET) $(INFERENCE_SRCS) \
	$(OPENCV_LIBS) $(NCNN_LIBS) -fopenmp -lm

# Clean build files
clean:
	rm -f $(C_TARGET) $(CAPTURE_TARGET) $(INFERENCE_TARGET)
