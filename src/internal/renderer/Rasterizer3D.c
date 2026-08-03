#include "Rasterizer3D.h"
#include "RasterizerCommon.h"
#include "graphics/Mesh.h"
#include "graphics/Texture.h"
#include "graphics/Vertex.h"
#include "graphics/VertexOut.h"
#include "math/Matrix4.h"
#include "math/Vector3.h"
#include "math/Vector4.h"
#include "math/common.h"
#include "scene/Transform.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static VertexOut* project_Object3D_vertices_to_screen(
    const Camera* camera, const Transform* transform,
    const Vertex* mesh_vertices, u32 vertex_count, i32 width, i32 height);
static VertexOut ndc_to_screen(Vector3 ndc, i32 width, i32 height);
static void barycentric_rasterization(const VertexOut* a_out,
                                      const VertexOut* b_out,
                                      const VertexOut* c_out,
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
    Texture* checker = NULL;
    if (!Texture_create_checker(&checker))
        return;
    /* --- Just temporary */

    for (u32 i = 0; i < index_count; i += 3) {
        VertexOut a = projected_vertices[mesh_indices[i]];
        VertexOut b = projected_vertices[mesh_indices[i + 1]];
        VertexOut c = projected_vertices[mesh_indices[i + 2]];
        barycentric_rasterization(&a, &b, &c, checker, frame_buffer,
                                  depth_buffer, width, height);
    }

    free(projected_vertices);
    Texture_destroy(&checker);
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
        // Clipping
        // if (clip.x < -clip.w || clip.x >
        // clip.w)
        //     continue;
        // if (clip.y < -clip.w || clip.y >
        // clip.w)
        //     continue;
        // if (clip.z < -clip.w || clip.z >
        // clip.w)
        //     continue;
        // Perspective divide
        Vector3 ndc = {clip.x / clip.w, clip.y / clip.w, clip.z / clip.w};
        // Normalized Device Coordinates
        // (NDC) -> Screen coordinates
        projected_vertices[i] = ndc_to_screen(ndc, width, height);
        projected_vertices[i].uv = mesh_vertices[i].uv;
    }
    return projected_vertices;
}

static VertexOut ndc_to_screen(Vector3 ndc, i32 width, i32 height) {
    return (VertexOut){.screen = {.x = (ndc.x + 1.0f) * width * 0.5f,
                                  .y = (1.0f - ndc.y) * height * 0.5f},
                       .depth = (ndc.z + 1.0f) * 0.5f};
}

static void barycentric_rasterization(const VertexOut* a_out,
                                      const VertexOut* b_out,
                                      const VertexOut* c_out,
                                      const Texture* texture,
                                      Color* frame_buffer, f32* depth_buffer,
                                      i32 width, i32 height) {
    // Color colors[3] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE};
    Vector2 a = {a_out->screen.x, a_out->screen.y};
    Vector2 b = {b_out->screen.x, b_out->screen.y};
    Vector2 c = {c_out->screen.x, c_out->screen.y};

    // Find bounding box with all the candidate pixels
    i32 x_min = floor(Math_min(a.x, (Math_min(b.x, c.x))));
    i32 y_min = floor(Math_min(a.y, Math_min(b.y, c.y)));
    i32 x_max = ceil(Math_max(a.x, (Math_max(b.x, c.x))));
    i32 y_max = ceil(Math_max(a.y, Math_max(b.y, c.y)));
    // Check boundaries and clip triangle if necessary
    x_min = Math_max(x_min, 0);
    y_min = Math_max(y_min, 0);
    x_max = Math_min(x_max, width - 1);
    y_max = Math_min(y_max, height - 1);

    // Compute the area of the parallelogram
    f32 area = edge(a, b, c);

    // Stick to top-left rule filling convention
    f32 bias0 = is_top_left(b, c) ? 0 : -0.0001;
    f32 bias1 = is_top_left(c, a) ? 0 : -0.0001;
    f32 bias2 = is_top_left(a, b) ? 0 : -0.0001;

    // Compute the cosntant delta values that will be used for horizontal a
    // vertical steps in order to avoid computing edge function each
    // iteration
    f32 delta_w0_col = b.y - c.y;
    f32 delta_w1_col = c.y - a.y;
    f32 delta_w2_col = a.y - b.y;

    f32 delta_w0_row = c.x - b.x;
    f32 delta_w1_row = a.x - c.x;
    f32 delta_w2_row = b.x - a.x;

    // Compute edge function to see if pixel is inside triangle
    Vector2 p = {x_min + 0.5f, y_min + 0.5f};
    f32 w0_row = edge(b, c, p) + bias0;
    f32 w1_row = edge(c, a, p) + bias1;
    f32 w2_row = edge(a, b, p) + bias2;

    bool is_inside;

    // Loop all candidate pixels inside the bounding box
    for (i32 y = y_min; y <= y_max; y++) {
        f32 w0 = w0_row;
        f32 w1 = w1_row;
        f32 w2 = w2_row;
        for (i32 x = x_min; x <= x_max; x++) {
            if (area >= 0.0f) {
                is_inside = w0 >= 0 && w1 >= 0 && w2 >= 0;
            } else {
                is_inside = w0 <= 0 && w1 <= 0 && w2 <= 0;
            }
            if (is_inside) {
                // Compute barycentric coordinates alpha, beta and gamma
                f32 alpha = w0 / area;
                f32 beta = w1 / area;
                f32 gamma = w2 / area;
                // u8 r = alpha * Color_get_red(colors[0]) +
                //        beta * Color_get_red(colors[1]) +
                //        gamma * Color_get_red(colors[2]);
                // u8 g = alpha * Color_get_green(colors[0]) +
                //        beta * Color_get_green(colors[1]) +
                //        gamma * Color_get_green(colors[2]);
                // u8 b = alpha * Color_get_blue(colors[0]) +
                //        beta * Color_get_blue(colors[1]) +
                //        gamma * Color_get_blue(colors[2]);
                // UV interpolation (texture mapping)
                f32 u = alpha * a_out->uv.x + beta * b_out->uv.x +
                        gamma * c_out->uv.x;
                f32 v = alpha * a_out->uv.y + beta * b_out->uv.y +
                        gamma * c_out->uv.y;
                Color color = Texture_sample(texture, u, v);
                // Z interpolation
                f32 z = alpha * a_out->depth + beta * b_out->depth +
                        gamma * c_out->depth;
                // if (z < depth_buffer[y * width + x]) {
                //     depth_buffer[y * width + x] = z;
                put_pixel(x, y, COLOR_WHITE, frame_buffer, width);
                // }
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
