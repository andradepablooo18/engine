#ifndef VECTOR4_H
#define VECTOR4_H

#include "core/types.h"
#include "math/Vector3.h"

typedef union Vector4 {
        struct {
                f32 x;
                f32 y;
                f32 z;
                f32 w;
        };
        f32 v4[4];
} Vector4;

Vector4 Vector4_from_vector3(Vector3 v);

#endif
