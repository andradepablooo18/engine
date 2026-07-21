#ifndef VECTOR3_H
#define VECTOR3_H

#include "core/types.h"
#include <stdbool.h>

typedef struct Vector3 {
        f32 x;
        f32 y;
        f32 z;
} Vector3;

Vector3 Vector3_create(f32 x, f32 y, f32 z);
f32 Vector3_length(Vector3 v);
Vector3 Vector3_add(Vector3 a, Vector3 b);
Vector3 Vector3_sub(Vector3 a, Vector3 b);
Vector3 Vector3_scale(Vector3 v, f32 scalar);
void Vector3_scale_in_place(Vector3* v, f32 scalar);
Vector3 Vector3_normalize(Vector3 v);
void Vector3_normalize_in_place(Vector3* v);
f32 Vector3_distance(Vector3 a, Vector3 b);
bool Vector3_equals(Vector3 a, Vector3 b, f32 epsilon);
f32 Vector3_dot_product(Vector3 a, Vector3 b);
Vector3 Vector3_cross_product(Vector3 a, Vector3 b);

#endif
