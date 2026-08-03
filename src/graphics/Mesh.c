#include "graphics/Mesh.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Mesh {
        Vertex* vertices;
        u32* indices;
        u32 vertex_count;
        u32 index_count;
};

bool Mesh_create_cube(Mesh** self) {
    assert(self);
    *self = calloc(1, sizeof(Mesh));
    if (!*self) {
        fprintf(stderr, "Mesh_create_cube: allocation failed\n\n");
        return false;
    }
    Mesh* s = *self;
    s->vertices = calloc(8, sizeof(Vertex));
    s->vertex_count = 8;

    s->indices = calloc(36, sizeof(u32));
    s->index_count = 36;

    s->vertices[0] = (Vertex){.position = {-1, -1, -1}, .uv = {0.0f, 0.0f}};
    s->vertices[1] = (Vertex){.position = {1, -1, -1}, .uv = {0.0f, 0.0f}};
    s->vertices[2] = (Vertex){.position = {-1, 1, -1}, .uv = {0.0f, 0.0f}};
    s->vertices[3] = (Vertex){.position = {1, 1, -1}, .uv = {0.0f, 0.0f}};

    s->vertices[4] = (Vertex){.position = {-1, -1, 1}, .uv = {0.0f, 0.0f}};
    s->vertices[5] = (Vertex){.position = {1, -1, 1}, .uv = {0.0f, 0.0f}};
    s->vertices[6] = (Vertex){.position = {-1, 1, 1}, .uv = {0.0f, 0.0f}};
    s->vertices[7] = (Vertex){.position = {1, 1, 1}, .uv = {0.0f, 0.0f}};

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

// Just for testing
bool Mesh_create_quad(Mesh** self) {
    assert(self);
    *self = calloc(1, sizeof(Mesh));
    if (!*self) {
        fprintf(stderr, "Mesh_create_quad: allocation failed\n\n");
        return false;
    }
    Mesh* s = *self;
    s->vertices = calloc(4, sizeof(Vertex));
    s->vertex_count = 4;

    s->indices = calloc(6, sizeof(u32));
    s->index_count = 6;

    s->vertices[0] = (Vertex){.position = {-1, 1, 0}, .uv = {0.0f, 0.0f}};
    s->vertices[1] = (Vertex){.position = {1, 1, 0}, .uv = {1.0f, 0.0f}};
    s->vertices[2] = (Vertex){.position = {1, -1, 0}, .uv = {1.0f, 1.0f}};
    s->vertices[3] = (Vertex){.position = {-1, -1, 0}, .uv = {0.0f, 1.0f}};

    u32 indices[] = {
        0, 2, 3, 0, 1, 2,
    };

    memcpy(s->indices, indices, sizeof(indices));
    return true;
}

void Mesh_destroy(Mesh** self) {
    assert(self);
    if (!*self)
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

const Vertex* Mesh_get_vertices(const Mesh* self) {
    assert(self);
    return self->vertices;
}

u32 Mesh_get_vertex_count(const Mesh* self) {
    assert(self);
    return self->vertex_count;
}

const u32* Mesh_get_indices(const Mesh* self) {
    assert(self);
    return self->indices;
}

u32 Mesh_get_index_count(const Mesh* self) {
    assert(self);
    return self->index_count;
}
