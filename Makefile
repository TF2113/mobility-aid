CC = gcc

# Compiler and Linker Flags
CFLAGS = -Wall -Wextra -O2 -Iinclude -Isrc/utils
TFLAGS = -Itest/framework
LDLIBS = -lsqlite3

# Targets
C_TARGET = builds/mobility_aid
VIB_TARGET = builds/vibrate
TEST_TARGET = builds/test_runner

# Source files
C_SOURCES = src/prox_sensor.c src/utils/tick.c src/utils/gpio_functions.c src/utils/config_db.c
VIB_SOURCES = src/vibrate.c src/utils/gpio_functions.c
TEST_SOURCES = test/test_vibrate.c test/vibrate_mock.c test/framework/unity.c

# Phony targets
.PHONY: all clean

all: $(C_TARGET) $(VIB_TARGET) $(TEST_TARGET)

# C program
$(C_TARGET): $(C_SOURCES)
	$(CC) $(CFLAGS) -o $@ $(C_SOURCES) $(LDLIBS)

# Vibrate program
$(VIB_TARGET): $(VIB_SOURCES)
	$(CC) $(CFLAGS) -o $@ $(VIB_SOURCES)

# Test runner
$(TEST_TARGET): $(TEST_SOURCES)
	$(CC) $(CFLAGS) $(TFLAGS) -o $@ $(TEST_SOURCES)

# Clean build files
clean:
	rm -f $(C_TARGET) $(VIB_TARGET) $(TEST_TARGET)