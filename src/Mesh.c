#include "Mesh.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Mesh {
        Vector3* vertices;
        u32 vertex_count;
        u32* indices;
        u32 index_count;
};

bool Mesh_create_cube(Mesh** self) {
    if (!self) {
        fprintf(stderr, "Mesh_create_cube: argument is NULL\n");
        return false;
    }
    *self = calloc(1, sizeof(Mesh));
    if (!*self) {
        fprintf(stderr, "Mesh_create_cube: allocation failed\n\n");
        return false;
    }
    Mesh* s = *self;
    s->vertices = calloc(8, sizeof(Vector3));
    s->vertex_count = 8;

    s->indices = calloc(36, sizeof(u32));
    s->index_count = 36;

    s->vertices[0] = (Vector3){-1, -1, -1};
    s->vertices[1] = (Vector3){1, -1, -1};
    s->vertices[2] = (Vector3){-1, 1, -1};
    s->vertices[3] = (Vector3){1, 1, -1};

    s->vertices[4] = (Vector3){-1, -1, 1};
    s->vertices[5] = (Vector3){1, -1, 1};
    s->vertices[6] = (Vector3){-1, 1, 1};
    s->vertices[7] = (Vector3){1, 1, 1};

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

    memcpy(s->indices, indices, sizeof(indices));
    return true;
}

void Mesh_destroy(Mesh** self) {
    if (!self || !*self)
        return;
    Mesh* const s = *self;
    if (s->vertices) {
        free(s->vertices);
        s->vertices = NULL;
    }
    if (s->indices) {
        free(s->indices);
        s->indices = NULL;
    }
    free(s);
    *self = NULL;
}

Vector3* Mesh_get_vertices(Mesh* self) {
    if (!self) {
        fprintf(stderr, "Mesh_get_vertices: Mesh* is NULL\n");
        return NULL;
    }
    return self->vertices;
}

u32 Mesh_get_vertex_count(Mesh* self) {
    if (!self) {
        fprintf(stderr, "Mesh_get_vertex_count: Mesh* is NULL\n");
        return -1;
    }
    return self->vertex_count;
}
