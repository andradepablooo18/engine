#ifndef COLOR_H
#define COLOR_H

#include "core/types.h"

typedef u32 Color;

enum {
    COLOR_BLACK = 0x000000FF,
    COLOR_WHITE = 0xFFFFFFFF,
    COLOR_RED = 0xFF0000FF,
    COLOR_GREEN = 0x00FF00FF,
    COLOR_BLUE = 0x0000FFFF,
    COLOR_YELLOW = 0xFFFF00FF,
    COLOR_CYAN = 0x00FFFFFF,
    COLOR_MAGENTA = 0xFF00FFFF
};

static inline Color Color_create(u8 r, u8 g, u8 b, u8 a) {
    return ((Color)r << 24) | ((Color)g << 16) | ((Color)b << 8) | (Color)a;
}

static inline u8 Color_get_red(Color color) { return (u8)(color >> 24); }

static inline u8 Color_get_green(Color color) { return (u8)(color >> 16); }

static inline u8 Color_get_blue(Color color) { return (u8)(color >> 8); }

static inline u8 Color_get_alpha(Color color) { return (u8)color; }

#endif
