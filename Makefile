CC := gcc
CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Iinclude
TARGET := jobtracker

SRC_DIR := src
BUILD_DIR := build

SOURCES := $(SRC_DIR)/main.c \
           $(SRC_DIR)/application.c \
           $(SRC_DIR)/file.c \
           $(SRC_DIR)/menu.c
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
