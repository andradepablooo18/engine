#ifndef RASTERIZERCOMMON_H
#define RASTERIZERCOMMON_H

/* Contains internal and reusable functions */

#include "core/Color.h"
#include "core/types.h"
#include "math/Vector2i.h"
#include <stdbool.h>

static inline void put_pixel(i32 x, i32 y, Color color, Color* frame_buffer,
                             i32 width) {
    frame_buffer[y * width + x] = color;
}

/*
 * This function assumes that triangles are traversed in Counter-Clockwise
 * (CCW) order and that they are located in screen space where y increases
 * downwards
 * */
static inline bool is_top_left(Vector2i a, Vector2i b) {
    /* Compute edge (i.e. resultat vector of b - a) */
    Vector2i edge = {b.x - a.x, b.y - a.y};
    /* Is top if is perfectly horizontal (dy == 0) and goes to the left (dx < 0)
     */
    bool is_top_edge = edge.y == 0 && edge.x < 0;
    /* Is left if it goes downwards (dy > 0) */
    bool is_left_edge = edge.y > 0;
    return is_top_edge || is_left_edge;
}

inline static i64 edge(Vector2i a, Vector2i b, Vector2i p) {
    Vector2i ab = {b.x - a.x, b.y - a.y};
    Vector2i ap = {p.x - a.x, p.y - a.y};
    return Vector2i_cross_product(ab, ap);
}

#endif
