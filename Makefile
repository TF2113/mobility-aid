CC = gcc

# Compiler and Linker Flags
CFLAGS = -Wall -Wextra -O2 -Iinclude -Isrc/utils
TFLAGS = -DUNITY_INCLUDE_DOUBLE -Itest/framework -Itest/mocks -Isrc/utils
LDLIBS = -lsqlite3

# Targets
C_TARGET = builds/mobility_aid
VIB_TARGET = builds/vibrate
CLI_TEST_TARGET = builds/test_cli_runner
DB_TEST_TARGET = builds/test_db_runner
GPIO_TEST_TARGET = builds/test_gpio_runner
LOGIC_TEST_TARGET = builds/test_logic_runner

# Source files
C_SOURCES = src/sys_controller.c src/utils/*.c
VIB_SOURCES = src/vibrate.c src/utils/gpio_functions.c
CLI_TEST_SOURCES = test/runners/test_cli_runner.c test/test_cli_vibrate.c test/mocks/vibrate_mock.c test/framework/unity.c
DB_TEST_SOURCES = test/runners/test_db_runner.c test/test_db_config.c src/utils/config_db.c test/framework/unity.c
GPIO_TEST_SOURCES = test/runners/test_gpio_runner.c test/test_gpio_functions.c src/utils/gpio_functions.c test/framework/unity.c
LOGIC_TEST_SOURCES = test/runners/test_logic_runner.c test/test_logic_vibrate.c test/mocks/vibrate_unit.c test/mocks/hw_mock.c test/framework/unity.c

# Phony targets
.PHONY: all clean test-run

all: $(C_TARGET) $(VIB_TARGET) $(CLI_TEST_TARGET) $(LOGIC_TEST_TARGET) $(DB_TEST_TARGET) $(GPIO_TEST_TARGET)
test-run: $(CLI_TEST_TARGET) $(LOGIC_TEST_TARGET) $(DB_TEST_TARGET) $(GPIO_TEST_TARGET)
	@echo "\nRunning Tests\n"
	@./$(CLI_TEST_TARGET)
	@echo ""
	@./$(LOGIC_TEST_TARGET)
	@echo ""
	@./$(DB_TEST_TARGET)
	@echo ""
	@./$(GPIO_TEST_TARGET)

# C program
$(C_TARGET): $(C_SOURCES)
	$(CC) $(CFLAGS) -o $@ $(C_SOURCES) $(LDLIBS)

# Vibrate program
$(VIB_TARGET): $(VIB_SOURCES)
	$(CC) $(CFLAGS) -o $@ $(VIB_SOURCES)

# Test runners
$(CLI_TEST_TARGET): $(CLI_TEST_SOURCES)
	$(CC) $(CFLAGS) $(TFLAGS) -o $@ $(CLI_TEST_SOURCES)

$(DB_TEST_TARGET): $(DB_TEST_SOURCES)
	$(CC) $(CFLAGS) $(TFLAGS) -o $@ $(DB_TEST_SOURCES) $(LDLIBS)

$(GPIO_TEST_TARGET): $(GPIO_TEST_SOURCES)
	$(CC) $(CFLAGS) $(TFLAGS) -o $@ $(GPIO_TEST_SOURCES)

$(LOGIC_TEST_TARGET): $(LOGIC_TEST_SOURCES)
	$(CC) $(CFLAGS) $(TFLAGS) -o $@ $(LOGIC_TEST_SOURCES)

# Clean build files
clean:
	rm -f $(C_TARGET) $(VIB_TARGET) $(CLI_TEST_TARGET) $(LOGIC_TEST_TARGET) $(DB_TEST_TARGET) $(GPIO_TEST_TARGET)