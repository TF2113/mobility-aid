CC = gcc

# Compiler and Linker Flags
CFLAGS = -Wall -Wextra -O2 -Iinclude -Isrc/utils
TFLAGS = -DUNITY_INCLUDE_DOUBLE -Itest/framework -Itest/mocks
LDLIBS = -lsqlite3

# Targets
C_TARGET = builds/mobility_aid
VIB_TARGET = builds/vibrate
CLI_TEST_TARGET = builds/test_cli_runner
LOGIC_TEST_TARGET = builds/test_logic_runner

# Source files
C_SOURCES = src/prox_sensor.c src/utils/tick.c src/utils/gpio_functions.c src/utils/config_db.c
VIB_SOURCES = src/vibrate.c src/utils/gpio_functions.c
CLI_TEST_SOURCES = test/test_cli_runner.c test/test_cli_vibrate.c test/mocks/vibrate_mock.c test/framework/unity.c
LOGIC_TEST_SOURCES = test/test_logic_runner.c test/test_logic_vibrate.c test/mocks/vibrate_unit.c test/mocks/hw_mock.c test/framework/unity.c

# Phony targets
.PHONY: all clean

all: $(C_TARGET) $(VIB_TARGET) $(CLI_TEST_TARGET) $(LOGIC_TEST_TARGET)

# C program
$(C_TARGET): $(C_SOURCES)
	$(CC) $(CFLAGS) -o $@ $(C_SOURCES) $(LDLIBS)

# Vibrate program
$(VIB_TARGET): $(VIB_SOURCES)
	$(CC) $(CFLAGS) -o $@ $(VIB_SOURCES)

# Test runners
$(CLI_TEST_TARGET): $(CLI_TEST_SOURCES)
	$(CC) $(CFLAGS) $(TFLAGS) -o $@ $(CLI_TEST_SOURCES)

$(LOGIC_TEST_TARGET): $(LOGIC_TEST_SOURCES)
	$(CC) $(CFLAGS) $(TFLAGS) -o $@ $(LOGIC_TEST_SOURCES)

# Clean build files
clean:
	rm -f $(C_TARGET) $(VIB_TARGET) $(CLI_TEST_TARGET) $(LOGIC_TEST_TARGET)