#ifndef VECTOR2_H
#define VECTOR2_H

#include <stdbool.h>

typedef struct Vector2 {
        float x;
        float y;
} Vector2;

Vector2 Vector2_create(float x, float y);
float Vector2_length(Vector2 v);
Vector2 Vector2_add(Vector2 a, Vector2 b);
Vector2 Vector2_sub(Vector2 a, Vector2 b);
Vector2 Vector2_scale(Vector2 v, float scalar);
void Vector2_scale_in_place(Vector2* v, float scalar);
Vector2 Vector2_normalize(Vector2 v);
void Vector2_normalize_in_place(Vector2* v);
float Vector2_distance(Vector2 a, Vector2 b);
bool Vector2_equals(Vector2 a, Vector2 b, float epsilon);
float Vector2_dot_product(Vector2 a, Vector2 b);
float Vector2_cross_product(Vector2 a, Vector2 b);

#endif
