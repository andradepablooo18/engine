CC = clang

TARGET = engine

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SOURCES = $(shell find $(SRC_DIR) -name "*.c")

DEBUG_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/debug/%.o,$(SOURCES))
RELEASE_OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/release/%.o,$(SOURCES))

DEBUG_BIN = $(BIN_DIR)/debug/$(TARGET)
RELEASE_BIN = $(BIN_DIR)/release/$(TARGET)

COMMON_FLAGS = -Wall -Wextra -std=c17 \
               -Iinclude \
               -Isrc \
               $(shell pkg-config --cflags sdl3)

DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O2 -DNDEBUG

LDFLAGS = $(shell pkg-config --libs sdl3)


.PHONY: debug release debug-run release-run clean


# =====================
# Debug
# =====================

debug: $(DEBUG_BIN)

$(DEBUG_BIN): $(DEBUG_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(DEBUG_OBJ) -o $@ $(LDFLAGS)


build/debug/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(COMMON_FLAGS) $(DEBUG_FLAGS) -c $< -o $@


# =====================
# Release
# =====================

release: $(RELEASE_BIN)

$(RELEASE_BIN): $(RELEASE_OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(RELEASE_OBJ) -o $@ $(LDFLAGS)


build/release/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(COMMON_FLAGS) $(RELEASE_FLAGS) -c $< -o $@


# =====================
# Run
# =====================

debug-run: debug
	./$(DEBUG_BIN)


release-run: release
	./$(RELEASE_BIN)


# =====================
# Clean
# =====================

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
