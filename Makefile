# Compiler
LIBS := -lGL -lGLEW -lglfw -lm
CC   := gcc
CFLAGS := -Wall -Wextra -ggdb
BUILD_DIR = ./build

all: breakout triangle triforce texture

breakout: $(BUILD_DIR)/breakout

triangle: $(BUILD_DIR)/triangle

triforce: $(BUILD_DIR)/triforce

texture: $(BUILD_DIR)/texture

transforms: $(BUILD_DIR)/transforms

$(BUILD_DIR)/transforms: $(BUILD_DIR)/transforms.o $(BUILD_DIR)/stb_image.o
	$(CC) $(LIBS) $(BUILD_DIR)/transforms.o $(BUILD_DIR)/stb_image.o -o $(BUILD_DIR)/transforms

$(BUILD_DIR)/transforms.o: src/transforms.c src/pl_math.h
	$(CC) $(CFLAGS) -c src/transforms.c -o $(BUILD_DIR)/transforms.o

$(BUILD_DIR)/texture: $(BUILD_DIR)/texture.o $(BUILD_DIR)/stb_image.o
	$(CC) $(LIBS) $(BUILD_DIR)/texture.o $(BUILD_DIR)/stb_image.o -o $(BUILD_DIR)/texture

$(BUILD_DIR)/stb_image.o: src/stb_image.c src/stb_image.h
	$(CC) $(CFLAGS) -c src/stb_image.c -o $(BUILD_DIR)/stb_image.o

$(BUILD_DIR)/texture.o: src/texture.c
	$(CC) $(CFLAGS) -c src/texture.c -o $(BUILD_DIR)/texture.o

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

.PHONY: clean re all breakout triangle triforce texture transforms
