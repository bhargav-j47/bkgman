#Configuration
CC = gcc
CFLAGS = -Wall -Wextra -O2 -Isrc
PREFIX ?= /usr/

#Directories
BUILD_DIR = build
SRC_DIR = src
TARGET = $(BUILD_DIR)/bkgman

#Files
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))
HEADERS = $(wildcard $(SRC_DIR)/*.h)

#Tasks
.PHONY: all build clean install

all: build

build: $(TARGET)

# Linking Rule
$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CC) $(CFLAGS) $(OBJS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(BUILD_DIR)
	@echo "Compiling $<..."
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

install: build
	@echo "Installing..."
	mkdir -p $(PREFIX)/bin
	cp $(TARGET) $(PREFIX)/bin/bkgman
	cp jpkg/jpkg $(PREFIX)/bin/jpkg
	chmod 755 $(PREFIX)/bin/bkgman
	chmod 755 $(PREFIX)/bin/jpkg
	
