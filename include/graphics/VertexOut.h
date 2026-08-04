#ifndef VERTEXPROJECTED_H
#define VERTEXPROJECTED_H

#include "math/Vector2.h"

typedef struct VertexOut {
        Vector2 screen;
        f32 depth;
        Vector2 uv;
        f32 inv_w;
} VertexOut;

#endif
