#include "math/Matrix4.h"
#include <math.h>

Matrix4 Matrix4_identity(void) {
    Matrix4 m4 = {{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
    return m4;
}

Matrix4 Matrix4_translation(f32 tx, f32 ty, f32 tz) {
    Matrix4 m4 = {{{1, 0, 0, tx}, {0, 1, 0, ty}, {0, 0, 1, tz}, {0, 0, 0, 1}}};
    return m4;
}

Matrix4 Matrix4_scale(f32 sx, f32 sy, f32 sz) {
    Matrix4 m4 = {{{sx, 0, 0, 0}, {0, sy, 0, 0}, {0, 0, sz, 0}, {0, 0, 0, 1}}};
    return m4;
}

Matrix4 Matrix4_rotation_x(f32 radians) {
    Matrix4 m4 = {{{1, 0, 0, 0},
                   {0, cos(radians), -sin(radians), 0},
                   {0, sin(radians), cos(radians), 0},
                   {0, 0, 0, 1}}};
    return m4;
}

Matrix4 Matrix4_rotation_y(f32 radians) {
    Matrix4 m4 = {{{cos(radians), 0, sin(radians), 0},
                   {0, 1, 0, 0},
                   {-sin(radians), 0, cos(radians), 0},
                   {0, 0, 0, 1}}};
    return m4;
}

Matrix4 Matrix4_rotation_z(f32 radians) {
    Matrix4 m4 = {{{cos(radians), -sin(radians), 0, 0},
                   {sin(radians), cos(radians), 0, 0},
                   {0, 0, 1, 0},
                   {0, 0, 0, 1}}};
    return m4;
}

// TODO: Matrix4 Matrix4_perspective(f32 fov, f32 aspect, f32 near, f32 far) {}

Matrix4 Matrix4_multiply(Matrix4 a, Matrix4 b) {
    Matrix4 c = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                c.m[i][j] += a.m[i][k] * b.m[k][j];
            }
        }
    }
    return c;
}

Vector4 Matrix4_multiply_vector(Matrix4 m4, Vector4 v) {
    Vector4 r = {0};
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            r.v4[i] += v.v4[j] * m4.m[i][j];
        }
    }
    return r;
}
