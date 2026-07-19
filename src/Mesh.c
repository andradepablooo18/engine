#include "Mesh.h"
#include "math/Vector3.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void Mesh_create_cube(Mesh* self) {
    if (!self) {
        fprintf(stderr, "Mesh_create_cube: Mesh* is NULL\n");
        return;
    }

    self->vertices = calloc(8, sizeof(Vector3));
    self->vertex_count = 8;

    self->indices = calloc(36, sizeof(u32));
    self->index_count = 36;

    self->vertices[0] = (Vector3){-1, -1, -1};
    self->vertices[1] = (Vector3){1, -1, -1};
    self->vertices[2] = (Vector3){-1, 1, -1};
    self->vertices[3] = (Vector3){1, 1, -1};

    self->vertices[4] = (Vector3){-1, -1, 1};
    self->vertices[5] = (Vector3){1, -1, 1};
    self->vertices[6] = (Vector3){-1, 1, 1};
    self->vertices[7] = (Vector3){1, 1, 1};

    u32 indices[] = {// Front (-Z)
                     0, 2, 1, 1, 2, 3,

                     // Back (+Z)
                     4, 5, 6, 5, 7, 6,

                     // Left (-X)
                     0, 4, 2, 2, 4, 6,

                     // Right (+X)
                     1, 3, 5, 3, 7, 5,

                     // Bottom (-Y)
                     0, 1, 4, 1, 5, 4,

                     // Top (+Y)
                     2, 6, 3, 3, 6, 7};

    memcpy(self->indices, indices, sizeof(indices));
}
