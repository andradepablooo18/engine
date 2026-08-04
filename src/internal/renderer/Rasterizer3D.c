#include "Rasterizer3D.h"
#include "RasterizerCommon.h"
#include "graphics/Mesh.h"
#include "graphics/Texture.h"
#include "graphics/Vertex.h"
#include "graphics/VertexOut.h"
#include "math/Matrix4.h"
#include "math/Vector2i.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/common.h"
#include "scene/Transform.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

/* Rasterizer3D works with 28.4 fixed point numbers */
/* (i.e. 16 possible states for fractional part) */
#define SUB_STEP 16
#define SUB_HALF 8

static VertexOut* project_Object3D_vertices_to_screen(
    const Camera* camera, const Transform* transform,
    const Vertex* mesh_vertices, u32 vertex_count, i32 width, i32 height);
static VertexOut ndc_to_screen(Vector3 ndc, i32 width, i32 height);
static void barycentric_rasterization(const VertexOut* a, const VertexOut* b,
                                      const VertexOut* c,
                                      const Texture* texture,
                                      Color* frame_buffer, f32* depth_buffer,
                                      i32 width, i32 height);

void Rasterizer3D_draw_object3D_solid(const Camera* camera, const Object3D* obj,
                                      Color* frame_buffer, f32* depth_buffer,
                                      i32 width, i32 height) {
    assert(frame_buffer);
    assert(depth_buffer);
    /* 1. Vertex Processing */
    const Transform* transform = Object3D_get_transform(obj);
    const Mesh* mesh = Object3D_get_mesh(obj);
    const Vertex* vertices = Mesh_get_vertices(mesh);
    u32 vertex_count = Mesh_get_vertex_count(mesh);

    VertexOut* projected_vertices = project_Object3D_vertices_to_screen(
        camera, transform, vertices, vertex_count, width, height);
    if (!projected_vertices) {
#ifdef DEBUG
        fprintf(stderr,
                "Rasterizer3D_draw_object3D_solid: allocation failed\n");
#endif
        return;
    }

    /* 2. Rasterization */
    const u32* mesh_indices = Mesh_get_indices(mesh);
    u32 index_count = Mesh_get_index_count(mesh);

    /* --- Just temporary */
    Texture* wood = NULL;
    if (!Texture_create(&wood, "assets/wood.png"))
        return;
    /* --- Just temporary */

    for (u32 i = 0; i < index_count; i += 3) {
        VertexOut a = projected_vertices[mesh_indices[i]];
        VertexOut b = projected_vertices[mesh_indices[i + 1]];
        VertexOut c = projected_vertices[mesh_indices[i + 2]];

        // VIEW FRUSTUM CULLING
        // If any triangle vertex is behing cameraa (w <= 0) all triangle is
        // discarded
        if (a.inv_w <= 0.0f || b.inv_w <= 0.0f || c.inv_w <= 0.0f) {
            continue;
        }
        barycentric_rasterization(&a, &b, &c, wood, frame_buffer, depth_buffer,
                                  width, height);
    }

    free(projected_vertices);
    Texture_destroy(&wood);
}

static VertexOut* project_Object3D_vertices_to_screen(
    const Camera* camera, const Transform* transform,
    const Vertex* mesh_vertices, u32 vertex_count, i32 width, i32 height) {
    // Get model_matrix from the transform of
    // the object
    Matrix4 model_matrix = Transform_get_matrix(transform);

    // Get view_matrix from the transform of
    // the camera
    Matrix4 view_matrix = Camera_get_view_matrix(camera);

    // Get camera members in order to get
    // projection matrix
    f32 fov_y = Camera_get_fov(camera);
    f32 near = Camera_get_near(camera);
    f32 far = Camera_get_far(camera);
    Matrix4 projection_matrix =
        Matrix4_perspective_projection(fov_y, width / (f32)height, near, far);

    VertexOut* projected_vertices = calloc(vertex_count, sizeof(VertexOut));
    if (!projected_vertices) {
        return NULL;
    }

    for (u32 i = 0; i < vertex_count; i++) {
        Vector4 local = Vector4_from_vector3(mesh_vertices[i].position);
        // Local space -> World space
        Vector4 world = Matrix4_multiply_vector(model_matrix, local);
        // World space -> Camera space
        Vector4 view = Matrix4_multiply_vector(view_matrix, world);
        // Camera space -> Clip space
        Vector4 clip = Matrix4_multiply_vector(projection_matrix, view);
        // Perspective divide
        Vector3 ndc = {clip.x / clip.w, clip.y / clip.w, clip.z / clip.w};
        // Normalized Device Coordinates
        // (NDC) -> Screen coordinates
        projected_vertices[i] = ndc_to_screen(ndc, width, height);
        projected_vertices[i].uv = mesh_vertices[i].uv;
        projected_vertices[i].inv_w = 1 / clip.w;
    }
    return projected_vertices;
}

static VertexOut ndc_to_screen(Vector3 ndc, i32 width, i32 height) {
    return (VertexOut){.screen = {.x = (ndc.x + 1.0f) * width * 0.5f,
                                  .y = (1.0f - ndc.y) * height * 0.5f},
                       .depth = (ndc.z + 1.0f) * 0.5f};
}

static void barycentric_rasterization(const VertexOut* a, const VertexOut* b,
                                      const VertexOut* c,
                                      const Texture* texture,
                                      Color* frame_buffer, f32* depth_buffer,
                                      i32 width, i32 height) {
    // 1. Convert screen coordinates to fixed point integers
    Vector2i a_i = {.x = (i32)roundf(a->screen.x * SUB_STEP),
                    .y = (i32)roundf(a->screen.y * SUB_STEP)};
    Vector2i b_i = {.x = (i32)roundf(b->screen.x * SUB_STEP),
                    .y = (i32)roundf(b->screen.y * SUB_STEP)};
    Vector2i c_i = {.x = (i32)roundf(c->screen.x * SUB_STEP),
                    .y = (i32)roundf(c->screen.y * SUB_STEP)};

    // 2. Compute bounding box (in real pixels) with all the candidate pixels
    i32 x_min = Math_max(0, Math_min(a_i.x, Math_min(b_i.x, c_i.x)) / SUB_STEP);
    i32 y_min = Math_max(0, Math_min(a_i.y, Math_min(b_i.y, c_i.y)) / SUB_STEP);
    // Add (SUB_STEP - 1) before division to ceil number
    i32 x_max = Math_min(
        width - 1,
        (Math_max(a_i.x, Math_max(b_i.x, c_i.x)) + SUB_STEP - 1) / SUB_STEP);
    i32 y_max = Math_min(
        height - 1,
        (Math_max(a_i.y, Math_max(b_i.y, c_i.y)) + SUB_STEP - 1) / SUB_STEP);

    // 3. Compute the area of the parallelogram in integers
    i64 area = edge(a_i, b_i, c_i);
    // 4. Backface Culling CCW
    // Do not draw triangle f the area is <= 0, the triangle is Clockwise (it is
    // facing away) or it is a line
    if (area <= 0)
        return;

    // 5. Stick to Top-left Rule.
    // If edge is not top_left then substract 1 from w to make w >= 0 fail
    i32 bias0 = is_top_left(b_i, c_i) ? 0 : -1;
    i32 bias1 = is_top_left(c_i, a_i) ? 0 : -1;
    i32 bias2 = is_top_left(a_i, b_i) ? 0 : -1;

    // 7. Compute the constant delta values that will be used for horizontal a
    // vertical steps in order to avoid computing edge function each
    // iteration
    i64 delta_w0_col = (i64)(b_i.y - c_i.y) * SUB_STEP;
    i64 delta_w1_col = (i64)(c_i.y - a_i.y) * SUB_STEP;
    i64 delta_w2_col = (i64)(a_i.y - b_i.y) * SUB_STEP;

    i64 delta_w0_row = (i64)(c_i.x - b_i.x) * SUB_STEP;
    i64 delta_w1_row = (i64)(a_i.x - c_i.x) * SUB_STEP;
    i64 delta_w2_row = (i64)(b_i.x - a_i.x) * SUB_STEP;

    // 8. Compute initial edge function to see if pixel is inside triangle
    Vector2i p = {x_min * SUB_STEP + SUB_HALF, y_min * SUB_STEP + SUB_HALF};
    i64 w0_row = edge(b_i, c_i, p) + bias0;
    i64 w1_row = edge(c_i, a_i, p) + bias1;
    i64 w2_row = edge(a_i, b_i, p) + bias2;

    f32 u_over_w_a = a->uv.x * a->inv_w;
    f32 v_over_w_a = a->uv.y * a->inv_w;

    f32 u_over_w_b = b->uv.x * b->inv_w;
    f32 v_over_w_b = b->uv.y * b->inv_w;

    f32 u_over_w_c = c->uv.x * c->inv_w;
    f32 v_over_w_c = c->uv.y * c->inv_w;

    // Loop all candidate pixels inside the bounding box
    for (i32 y = y_min; y <= y_max; y++) {
        i64 w0 = w0_row;
        i64 w1 = w1_row;
        i64 w2 = w2_row;
        for (i32 x = x_min; x <= x_max; x++) {
            bool is_inside = w0 >= 0 && w1 >= 0 && w2 >= 0;
            if (is_inside) {
                // Compute barycentric coordinates alpha, beta and gamma
                f32 alpha = w0 / (f32)area;
                f32 beta = w1 / (f32)area;
                f32 gamma = w2 / (f32)area;

                if (alpha < 0.0f || alpha > 1.0f || beta < 0.0f ||
                    beta > 1.0f || gamma < 0.0f || gamma > 1.0f) {
                    continue;
                }

                // Z interpolation
                f32 z = alpha * a->depth + beta * b->depth + gamma * c->depth;
                if (z < depth_buffer[y * width + x]) {
                    // UV interpolation (texture mapping)
                    f32 inv_w =
                        alpha * a->inv_w + beta * b->inv_w + gamma * c->inv_w;

                    f32 u_over_w = alpha * u_over_w_a + beta * u_over_w_b +
                                   gamma * u_over_w_c;
                    f32 v_over_w = alpha * v_over_w_a + beta * v_over_w_b +
                                   gamma * v_over_w_c;

                    f32 u = u_over_w / inv_w;
                    f32 v = v_over_w / inv_w;

                    Color color = Texture_sample(texture, u, v);

                    depth_buffer[y * width + x] = z;
                    put_pixel(x, y, color, frame_buffer, width);
                }
            }
            w0 += delta_w0_col;
            w1 += delta_w1_col;
            w2 += delta_w2_col;
        }
        w0_row += delta_w0_row;
        w1_row += delta_w1_row;
        w2_row += delta_w2_row;
    }
}
