#include "math/Vector2.h"
#include <math.h>

Vector2 Vector2_create(f32 x, f32 y) { return (Vector2){x, y}; }

f32 Vector2_length(Vector2 v) { return sqrtf(v.x * v.x + v.y * v.y); }

Vector2 Vector2_add(Vector2 a, Vector2 b) {
    return (Vector2){a.x + b.x, a.y + b.y};
}

Vector2 Vector2_sub(Vector2 a, Vector2 b) {
    return (Vector2){a.x - b.x, a.y - b.y};
}

Vector2 Vector2_scale(Vector2 v, f32 scalar) {
    return (Vector2){v.x * scalar, v.y * scalar};
}

void Vector2_scale_in_place(Vector2* v, f32 scalar) {
    if (!v)
        return;
    v->x *= scalar;
    v->y *= scalar;
}

f32 Vector2_distance(Vector2 a, Vector2 b) {
    return Vector2_length(Vector2_sub(a, b));
}

bool Vector2_equals(Vector2 a, Vector2 b, f32 epsilon) {
    return fabsf(a.x - b.x) < epsilon && fabsf(a.y - b.y) < epsilon;
}

Vector2 Vector2_normalize(Vector2 v) {
    f32 length = Vector2_length(v);
    return (Vector2){v.x / length, v.y / length};
}

void Vector2_normalize_in_place(Vector2* v) {
    if (!v)
        return;
    f32 length = Vector2_length(*v);
    v->x /= length;
    v->y /= length;
}

f32 Vector2_dot_product(Vector2 a, Vector2 b) { return a.x * b.x + a.y * b.y; }

f32 Vector2_cross_product(Vector2 a, Vector2 b) {
    return a.x * b.y - a.y * b.x;
}
