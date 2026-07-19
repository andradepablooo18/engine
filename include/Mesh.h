#ifndef MESH_H
#define MESH_H

#include "math/Vector3.h"
#include "types.h"

typedef struct Mesh {
        Vector3* vertices;
        u32 vertex_count;
        u32* indices;
        u32 index_count;
} Mesh;

void Mesh_create_cube(Mesh* self);

#endif
