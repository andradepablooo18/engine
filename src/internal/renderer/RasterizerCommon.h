#ifndef RASTERIZERCOMMON_H
#define RASTERIZERCOMMON_H

/* Contains internal and reusable functions */

#include "core/Color.h"
#include "core/types.h"
#include "math/Vector2.h"
#include "math/common.h"
#include <stdbool.h>

static inline void put_pixel(i32 x, i32 y, Color color, Color* frame_buffer,
                             i32 width) {
    frame_buffer[y * width + x] = color;
}

static inline bool is_top_left(Vector2 a, Vector2 b) {
    Vector2 edge = {b.x - a.x, b.y - a.y};
    bool is_top_edge = edge.y < MATH_EPSILON && edge.x > 0;
    bool is_left_edge = edge.y < 0;
    return is_top_edge || is_left_edge;
}

inline static f32 edge(Vector2 a, Vector2 b, Vector2 p) {
    Vector2 ab = {b.x - a.x, b.y - a.y};
    Vector2 ap = {p.x - a.x, p.y - a.y};
    return Vector2_cross_product(ab, ap);
}

#endif
