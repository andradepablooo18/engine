#ifndef VECTOR2I_H
#define VECTOR2I_H

#include "core/types.h"

typedef struct Vector2i {
        i32 x;
        i32 y;
} Vector2i;

static inline i64 Vector2i_cross_product(Vector2i a, Vector2i b) {
    return (i64)a.x * b.y - (i64)a.y * b.x;
}

#endif
