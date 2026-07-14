CC = gcc

TARGET = bin/engine

CFLAGS = -Wall -Wextra -std=c99 \
		 -Iinclude \
		 -Isrc \
		 $(shell pkg-config --cflags sdl3)

LDFLAGS = $(shell pkg-config --libs sdl3)

SRC := $(shell find src -name "*.c")
OBJ := $(patsubst src/%.c,build/%.o,$(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p bin
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

build/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build bin

run: all
	./$(TARGET)

.PHONY: all clean run
