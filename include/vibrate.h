#ifndef VIBRATE_H
#define VIBRATE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/file.h>
#include "gpio_functions.h"

int vibrate(int count, double duration, double delay);
int cli_vibrate(int argc, char *argv[]);

#endif