CC = gcc
CFLAGS = -Wall -Wextra
LIBS = -lz
SRC_DIR = src
BUILD_DIR = build

# List the source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Generate corresponding object file names
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# Target: program
all: $(BUILD_DIR)/program

# Rule to build the program
$(BUILD_DIR)/program: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

# Create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Target: clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
