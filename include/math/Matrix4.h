#ifndef MATRIX4_H
#define MATRIX4_H

#include "Vector4.h"
#include <types.h>

typedef struct Matrix4 {
        f32 m[4][4];
} Matrix4;

Matrix4 Matrix4_identity(void);
Matrix4 Matrix4_translation(f32 tx, f32 ty, f32 tz);
Matrix4 Matrix4_scale(f32 sx, f32 sy, f32 sz);
Matrix4 Matrix4_rotation_x(f32 radians);
Matrix4 Matrix4_rotation_y(f32 radians);
Matrix4 Matrix4_rotation_z(f32 radians);
Matrix4 Matrix4_perspective_projection(f32 fov_y, f32 aspect, f32 near,
                                       f32 far);
Matrix4 Matrix4_multiply(Matrix4 a, Matrix4 b);
Vector4 Matrix4_multiply_vector(Matrix4 m4, Vector4 v);

#endif
