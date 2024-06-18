# Compiler
LIBS := -lGL -lGLEW -lglfw -lm
CC   := gcc
CFLAGS := -Wall -Wextra
BUILD_DIR = ./build

all: breakout triangle triforce

breakout: $(BUILD_DIR)/breakout

triangle: $(BUILD_DIR)/triangle

triforce: $(BUILD_DIR)/triforce

$(BUILD_DIR)/breakout: $(BUILD_DIR)/breakout.o
	$(CC) $(LIBS) $(BUILD_DIR)/breakout.o -o $(BUILD_DIR)/breakout

$(BUILD_DIR)/breakout.o: src/breakout.c
	$(CC) $(CFLAGS) -c src/breakout.c -o $(BUILD_DIR)/breakout.o

$(BUILD_DIR)/triangle: $(BUILD_DIR)/triangle.o
	$(CC) $(LIBS) $(BUILD_DIR)/triangle.o -o $(BUILD_DIR)/triangle

$(BUILD_DIR)/triangle.o: src/triangle.c
	$(CC) $(CFLAGS) -c src/triangle.c -o $(BUILD_DIR)/triangle.o

$(BUILD_DIR)/triforce: $(BUILD_DIR)/triforce.o
	$(CC) $(LIBS) $(BUILD_DIR)/triforce.o -o $(BUILD_DIR)/triforce

$(BUILD_DIR)/triforce.o: src/triforce.c res/triforce/simple.frag res/triforce/simple.vert
	$(CC) $(CFLAGS) -c src/triforce.c -o $(BUILD_DIR)/triforce.o

clean:
	rm ./build/*

.PHONY: clean re all breakout triangle triforce
