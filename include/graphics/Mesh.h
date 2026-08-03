#ifndef MESH_H
#define MESH_H

#include "core/types.h"
#include "graphics/Vertex.h"
#include <stdbool.h>

typedef struct Mesh Mesh;

bool Mesh_create_cube(Mesh** self);
// Just for testing
bool Mesh_create_quad(Mesh** self);
void Mesh_destroy(Mesh** self);

const Vertex* Mesh_get_vertices(const Mesh* self);
u32 Mesh_get_vertex_count(const Mesh* self);
const u32* Mesh_get_indices(const Mesh* self);
u32 Mesh_get_index_count(const Mesh* self);

#endif
