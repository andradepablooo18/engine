#ifndef MESH_H
#define MESH_H

#include "math/Vector3.h"
#include "types.h"
#include <stdbool.h>

typedef struct Mesh Mesh;

bool Mesh_create_cube(Mesh** self);
void Mesh_destroy(Mesh** self);
Vector3* Mesh_get_vertices(Mesh* self);
u32 Mesh_get_vertex_count(Mesh* self);

#endif
