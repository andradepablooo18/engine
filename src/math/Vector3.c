#include "math/Vector3.h"
#include <math.h>

Vector3 Vector3_create(float x, float y, float z) { return (Vector3){x, y, z}; }

float Vector3_length(Vector3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Vector3_add(Vector3 a, Vector3 b) {
    return (Vector3){a.x + b.x, a.y + b.y, a.z + b.z};
}

Vector3 Vector3_sub(Vector3 a, Vector3 b) {
    return (Vector3){a.x - b.x, a.y - b.y, a.z - b.z};
}

Vector3 Vector3_scale(Vector3 v, float scalar) {
    return (Vector3){v.x * scalar, v.y * scalar, v.z * scalar};
}

void Vector3_scale_in_place(Vector3* v, float scalar) {
    if (!v)
        return;

    v->x *= scalar;
    v->y *= scalar;
    v->z *= scalar;
}

float Vector3_distance(Vector3 a, Vector3 b) {
    return Vector3_length(Vector3_sub(a, b));
}

bool Vector3_equals(Vector3 a, Vector3 b, float epsilon) {
    return fabsf(a.x - b.x) < epsilon && fabsf(a.y - b.y) < epsilon &&
           fabsf(a.z - b.z) < epsilon;
}

Vector3 Vector3_normalize(Vector3 v) {
    float length = Vector3_length(v);
    return (Vector3){v.x / length, v.y / length, v.z / length};
}

void Vector3_normalize_in_place(Vector3* v) {
    if (!v)
        return;

    float length = Vector3_length(*v);
    v->x /= length;
    v->y /= length;
    v->z /= length;
}

float Vector3_dot_product(Vector3 a, Vector3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 Vector3_cross_product(Vector3 a, Vector3 b) {
    return (Vector3){a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                     a.x * b.y - a.y * b.x};
}
