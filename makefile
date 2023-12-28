CC = gcc
CFLAGS = -Wall -Wextra --coverage
LIBS = -lz
SRC_DIR = src
BUILD_DIR = build
COVERAGE_DIR = coverage  # New directory for coverage data

# List the source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate corresponding object file names
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Target: ctar
all: $(BUILD_DIR)/ctar

# Rule to build the ctar
$(BUILD_DIR)/ctar: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Target: coverage
coverage: CFLAGS += -g -O0
coverage: all
	./$(BUILD_DIR)/ctar -l ./build/samples.tar.gz
	./$(BUILD_DIR)/ctar -l ./build/test.tar
	./$(BUILD_DIR)/ctar -d ./build/test -e ./build/test.tar 
	./$(BUILD_DIR)/ctar -c ./build/my.tar ./build/test
	./$(BUILD_DIR)/ctar -c ./build/my.tar.gz ./build/test
	./$(BUILD_DIR)/ctar -zc ./build/this.tar ./build/test
	./$(BUILD_DIR)/ctar -h
	./$(BUILD_DIR)/ctar -v
	./$(BUILD_DIR)/ctar

	gcov -o $(BUILD_DIR) $(SRCS)  # Generate coverage reports
	mv *.gcov $(COVERAGE_DIR)  # Move the generated coverage files to the coverage directory

# Target: clean
clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(COVERAGE_DIR)

.PHONY: all clean coverage
