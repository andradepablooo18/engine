#ifndef VECTOR2_H
#define VECTOR2_H

#include <stdbool.h>
#include <types.h>

typedef struct Vector2 {
        f32 x;
        f32 y;
} Vector2;

Vector2 Vector2_create(f32 x, f32 y);
f32 Vector2_length(Vector2 v);
Vector2 Vector2_add(Vector2 a, Vector2 b);
Vector2 Vector2_sub(Vector2 a, Vector2 b);
Vector2 Vector2_scale(Vector2 v, f32 scalar);
void Vector2_scale_in_place(Vector2* v, f32 scalar);
Vector2 Vector2_normalize(Vector2 v);
void Vector2_normalize_in_place(Vector2* v);
f32 Vector2_distance(Vector2 a, Vector2 b);
bool Vector2_equals(Vector2 a, Vector2 b, f32 epsilon);
f32 Vector2_dot_product(Vector2 a, Vector2 b);
f32 Vector2_cross_product(Vector2 a, Vector2 b);

#endif
