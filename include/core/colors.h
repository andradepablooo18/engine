#ifndef COLORS_H
#define COLORS_H

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

#define RGBA(r, g, b, a)                                                       \
    (((u32)(r) >> 24) | ((u32)(g) >> 16) | ((u32)(b) >> 8) | (u32)(a))

#endif
