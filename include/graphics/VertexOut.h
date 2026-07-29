#ifndef VERTEXPROJECTED_H
#define VERTEXPROJECTED_H

#include "math/Vector2.h"
#include "math/Vector3.h"

typedef struct VertexOut {
        Vector3 position;
} VertexOut;

static inline void VertexOut_swap(VertexOut* a, VertexOut* b) {
    VertexOut temp = *a;
    *a = *b;
    *b = temp;
}

#endif
