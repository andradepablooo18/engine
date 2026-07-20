#include "math/Matrix4.h"
#include <math.h>

Matrix4 Matrix4_identity(void) {
    return (Matrix4){{{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}}};
}

Matrix4 Matrix4_translation(f32 tx, f32 ty, f32 tz) {
    return (Matrix4){
        {{1, 0, 0, tx}, {0, 1, 0, ty}, {0, 0, 1, tz}, {0, 0, 0, 1}}};
}

Matrix4 Matrix4_scale(f32 sx, f32 sy, f32 sz) {
    return (Matrix4){
        {{sx, 0, 0, 0}, {0, sy, 0, 0}, {0, 0, sz, 0}, {0, 0, 0, 1}}};
}

Matrix4 Matrix4_rotation_x(f32 radians) {
    return (Matrix4){{{1, 0, 0, 0},
                      {0, cosf(radians), -sinf(radians), 0},
                      {0, sinf(radians), cosf(radians), 0},
                      {0, 0, 0, 1}}};
}

Matrix4 Matrix4_rotation_y(f32 radians) {
    return (Matrix4){{{cosf(radians), 0, sinf(radians), 0},
                      {0, 1, 0, 0},
                      {-sinf(radians), 0, cosf(radians), 0},
                      {0, 0, 0, 1}}};
}

Matrix4 Matrix4_rotation_z(f32 radians) {
    return (Matrix4){{{cosf(radians), -sinf(radians), 0, 0},
                      {sinf(radians), cosf(radians), 0, 0},
                      {0, 0, 1, 0},
                      {0, 0, 0, 1}}};
}

Matrix4 Matrix4_perspective_projection(f32 fov_y, f32 aspect, f32 near,
                                       f32 far) {
    f32 f = 1.0f / tanf(fov_y / 2);
    return (Matrix4){
        {{f / aspect, 0, 0, 0},
         {0, f, 0, 0},
         {0, 0, (far + near) / (far - near), (-2 * far * near) / (far - near)},
         {0, 0, -1, 0}}};
}

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
