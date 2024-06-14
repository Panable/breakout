# Compiler
LIBS := -lGL -lGLEW -lglfw
CC   := gcc
CFLAGS := -Wall -Wextra
BUILD_DIR = ./build

all: breakout triangle

breakout: $(BUILD_DIR)/breakout

triangle: $(BUILD_DIR)/triangle

$(BUILD_DIR)/breakout: breakout.o
	$(CC) $(LIBS) $(BUILD_DIR)/breakout.o -o $(BUILD_DIR)/breakout

$(BUILD_DIR)/triangle: $(BUILD_DIR)/triangle.o
	$(CC) $(LIBS) $(BUILD_DIR)/triangle.o -o $(BUILD_DIR)/triangle

$(BUILD_DIR)/triangle.o: src/triangle.c
	$(CC) $(CFLAGS) -c src/triangle.c -o $(BUILD_DIR)/triangle.o

$(BUILD_DIR)/breakout.o: src/breakout.c
	$(CC) $(CFLAGS) -c src/breakout.c -o $(BUILD_DIR)/breakout.o

clean:
	rm ./build/*

.PHONY: clean re all breakout triangle
