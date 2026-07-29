CC = gcc

COMMON_FLAGS = -Wall -Wextra -std=c99 \
               -Iinclude \
               -Isrc \
               $(shell pkg-config --cflags sdl3)

DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

LDFLAGS = $(shell pkg-config --libs sdl3)

SRC := $(shell find src -name "*.c")

DEBUG_OBJ := $(patsubst src/%.c,build/debug/%.o,$(SRC))
RELEASE_OBJ := $(patsubst src/%.c,build/release/%.o,$(SRC))

DEBUG_TARGET := bin/debug/engine
RELEASE_TARGET := bin/release/engine

# ==========================
# Debug
# ==========================

debug: $(DEBUG_TARGET)
	./$(DEBUG_TARGET)

$(DEBUG_TARGET): $(DEBUG_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(DEBUG_OBJ) -o $@ $(LDFLAGS)

build/debug/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(COMMON_FLAGS) $(DEBUG_FLAGS) -c $< -o $@

# ==========================
# Release
# ==========================

release: $(RELEASE_TARGET)
	./$(RELEASE_TARGET)

$(RELEASE_TARGET): $(RELEASE_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(RELEASE_OBJ) -o $@ $(LDFLAGS)

build/release/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(COMMON_FLAGS) $(RELEASE_FLAGS) -c $< -o $@

# ==========================
# Clean
# ==========================

clean:
	rm -rf build bin

.PHONY: debug release clean
