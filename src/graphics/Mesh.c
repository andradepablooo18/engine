#include "graphics/Mesh.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUAD_INDICES(i) i, i + 2, i + 1, i + 1, i + 2, i + 3

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
    s->vertices = calloc(24, sizeof(Vertex));
    s->vertex_count = 24;

    s->indices = calloc(36, sizeof(u32));
    s->index_count = 36;

    // Every face uses the same local vertex layout:
    //
    // 2 ------ 3
    // |        |
    // |        |
    // 0 ------ 1
    //
    // where:
    //
    // 0 = bottom-left
    // 1 = bottom-right
    // 2 = top-left
    // 3 = top-right
    //
    // "Left", "right", "top" and "bottom" are defined in the local space of
    // each face (not in world space), allowing every face to reuse the same UV
    // layout and the same index pattern nowing that every triangle vertex is
    // ordered is CCW in screen space:
    //
    // 0 -> 2 -> 1
    // 1 -> 2 -> 3

    // +Z (Front)
    s->vertices[0] = (Vertex){.position = {-1, -1, 1}, .uv = {0.0f, 1.0f}};
    s->vertices[1] = (Vertex){.position = {1, -1, 1}, .uv = {1.0f, 1.0f}};
    s->vertices[2] = (Vertex){.position = {-1, 1, 1}, .uv = {0.0f, 0.0f}};
    s->vertices[3] = (Vertex){.position = {1, 1, 1}, .uv = {1.0f, 0.0f}};

    // -Z (Back)
    s->vertices[4] = (Vertex){.position = {1, -1, -1}, .uv = {0.0f, 1.0f}};
    s->vertices[5] = (Vertex){.position = {-1, -1, -1}, .uv = {1.0f, 1.0f}};
    s->vertices[6] = (Vertex){.position = {1, 1, -1}, .uv = {0.0f, 0.0f}};
    s->vertices[7] = (Vertex){.position = {-1, 1, -1}, .uv = {1.0f, 0.0f}};

    // +Y (Top)
    s->vertices[8] = (Vertex){.position = {-1, 1, 1}, .uv = {0.0f, 1.0f}};
    s->vertices[9] = (Vertex){.position = {1, 1, 1}, .uv = {1.0f, 1.0f}};
    s->vertices[10] = (Vertex){.position = {-1, 1, -1}, .uv = {0.0f, 0.0f}};
    s->vertices[11] = (Vertex){.position = {1, 1, -1}, .uv = {1.0f, 0.0f}};

    // -Y (Bottom)
    s->vertices[12] = (Vertex){.position = {-1, -1, -1}, .uv = {0.0f, 1.0f}};
    s->vertices[13] = (Vertex){.position = {1, -1, -1}, .uv = {1.0f, 1.0f}};
    s->vertices[14] = (Vertex){.position = {-1, -1, 1}, .uv = {0.0f, 0.0f}};
    s->vertices[15] = (Vertex){.position = {1, -1, 1}, .uv = {1.0f, 0.0f}};

    // -X (Left)
    s->vertices[16] = (Vertex){.position = {-1, -1, -1}, .uv = {0.0f, 1.0f}};
    s->vertices[17] = (Vertex){.position = {-1, -1, 1}, .uv = {1.0f, 1.0f}};
    s->vertices[18] = (Vertex){.position = {-1, 1, -1}, .uv = {0.0f, 0.0f}};
    s->vertices[19] = (Vertex){.position = {-1, 1, 1}, .uv = {1.0f, 0.0f}};

    // +X (Right)
    s->vertices[20] = (Vertex){.position = {1, -1, 1}, .uv = {0.0f, 1.0f}};
    s->vertices[21] = (Vertex){.position = {1, -1, -1}, .uv = {1.0f, 1.0f}};
    s->vertices[22] = (Vertex){.position = {1, 1, 1}, .uv = {0.0f, 0.0f}};
    s->vertices[23] = (Vertex){.position = {1, 1, -1}, .uv = {1.0f, 0.0f}};

    u32 indices[] = {QUAD_INDICES(0),  QUAD_INDICES(4),  QUAD_INDICES(8),
                     QUAD_INDICES(12), QUAD_INDICES(16), QUAD_INDICES(20)};

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

    Mesh* m = *self;

    m->vertices = calloc(4, sizeof(Vertex));
    m->vertex_count = 4;

    m->indices = calloc(6, sizeof(u32));
    m->index_count = 6;

    m->vertices[0] = (Vertex){.position = {-1, 1, 0}, .uv = {0.0f, 0.0f}};
    m->vertices[1] = (Vertex){.position = {1, 1, 0}, .uv = {1.0f, 0.0f}};
    m->vertices[2] = (Vertex){.position = {1, -1, 0}, .uv = {1.0f, 1.0f}};
    m->vertices[3] = (Vertex){.position = {-1, -1, 0}, .uv = {0.0f, 1.0f}};

    u32 indices[] = {
        0, 2, 3, 0, 1, 2,
    };

    memcpy(m->indices, indices, sizeof(indices));
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
