#ifndef VERTEXPROJECTED_H
#define VERTEXPROJECTED_H

#include "math/Vector2.h"
#include "math/Vector3.h"

typedef struct VertexOut {
        Vector3 ndc;
        Vector2 screen;
} VertexOut;

#endif
