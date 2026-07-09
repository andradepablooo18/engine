#include "Vector2.h"
#include <math.h>

Vector2 Vector2_create(float x, float y) { return (Vector2){x, y}; }

float Vector2_length(Vector2 v) { return sqrtf(v.x * v.x + v.y * v.y); }

Vector2 Vector2_add(Vector2 a, Vector2 b) {
    return (Vector2){a.x + b.x, a.y + b.y};
}

Vector2 Vector2_sub(Vector2 a, Vector2 b) {
    return (Vector2){a.x - b.x, a.y - b.y};
}

Vector2 Vector2_scale(Vector2 v, float scalar) {
    return (Vector2){v.x * scalar, v.y * scalar};
}

void Vector2_scale_in_place(Vector2* v, float scalar) {
    v->x *= scalar;
    v->y *= scalar;
}

float Vector2_distance(Vector2 a, Vector2 b) {
    return Vector2_length(Vector2_sub(a, b));
}

bool Vector2_equals(Vector2 a, Vector2 b, float epsilon) {
    return fabsf(a.x - b.x) < epsilon && fabsf(a.y - b.y) < epsilon;
}

Vector2 Vector2_normalize(Vector2 v) {
    float length = Vector2_length(v);
    return (Vector2){v.x / length, v.y / length};
}

void Vector2_normalize_in_place(Vector2* v) {
    float length = Vector2_length(*v);
    v->x /= length;
    v->y /= length;
}

float Vector2_dot_product(Vector2 a, Vector2 b) {
    return a.x * b.x + a.y * b.y;
}

float Vector2_cross_product(Vector2 a, Vector2 b) {
    return a.x * b.y - a.y * b.x;
}
