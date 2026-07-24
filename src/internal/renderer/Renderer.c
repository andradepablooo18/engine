#include "Renderer.h"
#include "core/colors.h" // THIS SHOULD NOT BE HERE BRUH
#include "graphics/Camera.h"
#include "graphics/Mesh.h"
#include "graphics/RasterMode.h"
#include "graphics/VertexOut.h"
#include "math/common.h"
#include "scene/Object3D.h"
#include <math.h>
#include <stdlib.h>

struct Renderer {
        SDL_Renderer* handle;
        SDL_Texture* color_buffer_texture;
        u32* color_buffer;
        // f32* depth_buffer;
        i32 width;
        i32 height;
        RasterMode raster_mode;
};

/*
 *************************************************************************
 *************************************************************************
 *
 * Renderer_draw_line HELPERS (based on which algorithm it is used)
 *
 *************************************************************************
 *************************************************************************
 */

static void draw_line_DDA(Renderer* self, Vector2 start, Vector2 end,
                          u32 color);
static void draw_line_bresenham(Renderer* self, Vector2 start, Vector2 end,
                                u32 color);

/*
 *************************************************************************
 *************************************************************************
 *
 * Renderer_draw_triangle HELPERS (based on rasterization mode)
 *
 *************************************************************************
 *************************************************************************
 */

static void draw_triangle_points(Renderer* self, Triangle triangle, u32 color);
static void draw_triangle_wireframe(Renderer* self, Triangle triangle,
                                    u32 color);
static void draw_triangle_solid(Renderer* self, Triangle triangle, u32 color);

/*
 *************************************************************************
 *************************************************************************
 *
 * draw_triangle_solid HELPERS (based on which algorithm it is used)
 *
 *************************************************************************
 *************************************************************************
 */

static void scanline_rasterization(Renderer* self, Triangle triangle,
                                   u32 color);

static void barycentric_rasterization(Renderer* self, Triangle triangle,
                                      u32 color);

/*
 *************************************************************************
 *************************************************************************
 *
 * Renderer_draw_object3D HELPERS
 *
 *************************************************************************
 *************************************************************************
 */

static VertexOut* project_to_screen(const Renderer* self, const Camera* camera,
                                    const Mesh* mesh,
                                    const Transform* transform);
static Vector2 viewport_transform(const Renderer* self, Vector3 v);

/*
 *************************************************************************
 *************************************************************************
 *
 * Renderer_draw_object3D HELPERS (based on rasterization mode)
 *
 *************************************************************************
 *************************************************************************
 */

static void rasterize_points(Renderer* self, u32 vertex_count,
                             const VertexOut* projected_vertices);
static void rasterize_wireframe(Renderer* self, const Mesh* mesh,
                                const VertexOut* projected_vertices);
static void rasterize_solid(Renderer* self, const Mesh* Mesh,
                            const VertexOut* projected_vertices);

/*
 *************************************************************************
 *************************************************************************
 *
 * PUBLIC INTERNAL API
 *
 *************************************************************************
 *************************************************************************
 */

bool Renderer_create(Renderer** self, Window* window, RasterMode raster_mode) {
    *self = calloc(1, sizeof(Renderer));
    if (!*self) {
        fprintf(stderr, "Renderer_create: "
                        "allocation failed\n");
        return false;
    }

    Renderer* const s = *self;

    s->handle = SDL_CreateRenderer(Window_get_handle(window), NULL);
    if (!s->handle) {
        fprintf(stderr,
                "SDL_CreateRenderer failed: "
                "%s\n",
                SDL_GetError());
        Renderer_destroy(self);
        return false;
    }

    i32 width = Window_get_width(window);
    i32 height = Window_get_height(window);

    s->width = width;
    s->height = height;

    s->color_buffer_texture =
        SDL_CreateTexture(s->handle, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!s->color_buffer_texture) {
        fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
        Renderer_destroy(self);
        return false;
    }

    s->color_buffer = calloc(width * height, sizeof(u32));
    if (!s->color_buffer) {
        fprintf(stderr, "Renderer_create failed while "
                        "allocating memory for "
                        "color_buffer\n");
        Renderer_destroy(self);
        return false;
    }

    s->raster_mode = raster_mode;

    return true;
}

void Renderer_destroy(Renderer** self) {
    Renderer* const s = *self;
    if (s->color_buffer_texture) {
        SDL_DestroyTexture(s->color_buffer_texture);
        s->color_buffer_texture = NULL;
    }
    if (s->color_buffer) {
        free(s->color_buffer);
        s->color_buffer = NULL;
    }
    if (s->handle) {
        SDL_DestroyRenderer(s->handle);
        s->handle = NULL;
    }
    free(s);
    *self = NULL;
}

void Renderer_set_raster_mode(Renderer* self, RasterMode raster_mode) {
    self->raster_mode = raster_mode;
}

void Renderer_clear(Renderer* self, u32 color) {
    const size_t size = (size_t)self->width * self->height;
    for (size_t i = 0; i < size; i++) {
        self->color_buffer[i] = color;
    }
}

void Renderer_present(Renderer* self) {
    SDL_UpdateTexture(self->color_buffer_texture, NULL, self->color_buffer,
                      self->width * sizeof(u32));
    SDL_RenderTexture(self->handle, self->color_buffer_texture, NULL, NULL);
    SDL_RenderPresent(self->handle);
}

inline void Renderer_draw_pixel(Renderer* self, i32 x, i32 y, u32 color) {
    if (x < 0 || x >= self->width || y < 0 || y >= self->height)
        return;
    self->color_buffer[y * self->width + x] = color;
}

void Renderer_draw_line(Renderer* self, Vector2 start, Vector2 end, u32 color) {
    // draw_line_DDA(self, x1, y1, x2, y2, color);
    draw_line_bresenham(self, start, end, color);
}

void Renderer_draw_triangle(Renderer* self, Triangle triangle, u32 color) {
    switch (self->raster_mode) {
        case RASTER_MODE_POINTS:
            draw_triangle_points(self, triangle, color);
            break;
        case RASTER_MODE_WIREFRAME:
            draw_triangle_wireframe(self, triangle, color);
            break;
        case RASTER_MODE_SOLID:
            draw_triangle_solid(self, triangle, color);
            break;
        default:
            break;
    }
}

void Renderer_draw_object3D(Renderer* self, const Camera* camera,
                            const Object3D* obj) {
    // Vertex Processing
    const Mesh* mesh = Object3D_get_mesh(obj);
    const Transform transform = Object3D_get_transform(obj);
    VertexOut* projected_vertices =
        project_to_screen(self, camera, mesh, &transform);
    if (!projected_vertices)
        return;

    // Rasterization
    switch (self->raster_mode) {
        case RASTER_MODE_POINTS:
            rasterize_points(self, Mesh_get_vertex_count(mesh),
                             projected_vertices);
            break;
        case RASTER_MODE_WIREFRAME:
            rasterize_wireframe(self, mesh, projected_vertices);
            break;
        case RASTER_MODE_SOLID:
            rasterize_solid(self, mesh, projected_vertices);
            break;
        default:
            break;
    }

    free(projected_vertices);
}

/*
 *************************************************************************
 *************************************************************************
 *
 * STATIC HELPERS
 *
 *************************************************************************
 *************************************************************************
 */

static void draw_line_DDA(Renderer* self, Vector2 start, Vector2 end,
                          u32 color) {
    i32 x1 = (i32)roundf(start.x);
    i32 y1 = (i32)roundf(start.y);

    i32 x2 = (i32)roundf(end.x);
    i32 y2 = (i32)roundf(end.y);

    i32 dx = x2 - x1;
    i32 dy = y2 - y1;

    i32 abs_dx = abs(dx);
    i32 abs_dy = abs(dy);
    i32 steps = abs_dx > abs_dy ? abs_dx : abs_dy;

    if (steps == 0) {
        Renderer_draw_pixel(self, x1, y1, color);
        return;
    }

    f32 x_inc = dx / (f32)steps;
    f32 y_inc = dy / (f32)steps;

    f32 x = (f32)x1;
    f32 y = (f32)y1;

    for (i32 i = 0; i <= steps; i++) {
        Renderer_draw_pixel(self, (i32)roundf(x), (i32)roundf(y), color);
        x += x_inc;
        y += y_inc;
    }
}

static void draw_line_bresenham(Renderer* self, Vector2 start, Vector2 end,
                                u32 color) {
    i32 x1 = (i32)roundf(start.x);
    i32 y1 = (i32)roundf(start.y);

    i32 x2 = (i32)roundf(end.x);
    i32 y2 = (i32)roundf(end.y);

    i32 dx = abs(x2 - x1);
    i32 dy = abs(y2 - y1);

    i32 sx = x1 < x2 ? 1 : -1;
    i32 sy = y1 < y2 ? 1 : -1;

    i32 err = dx - dy;

    while (true) {
        Renderer_draw_pixel(self, x1, y1, color);

        if (x1 == x2 && y1 == y2)
            break;

        i32 e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

static void draw_triangle_points(Renderer* self, Triangle triangle, u32 color) {
    Renderer_draw_pixel(self, triangle.p0.x, triangle.p0.y, color);
    Renderer_draw_pixel(self, triangle.p1.x, triangle.p1.y, color);
    Renderer_draw_pixel(self, triangle.p2.x, triangle.p2.y, color);
}

static void draw_triangle_wireframe(Renderer* self, Triangle triangle,
                                    u32 color) {
    Renderer_draw_line(self, triangle.p0, triangle.p1, color);
    Renderer_draw_line(self, triangle.p1, triangle.p2, color);
    Renderer_draw_line(self, triangle.p0, triangle.p2, color);
}

static void draw_triangle_solid(Renderer* self, Triangle triangle, u32 color) {
    scanline_rasterization(self, triangle, color);
}

static void scanline_rasterization(Renderer* self, Triangle triangle,
                                   u32 color) {
    // Sort triangle vertices based on ints y
    // coordinate from smallest to biggest so
    // that p0.y <= p1.y <= p2.y
    if (triangle.p0.y > triangle.p1.y)
        Vector2_swap(&triangle.p0, &triangle.p1);
    if (triangle.p1.y > triangle.p2.y)
        Vector2_swap(&triangle.p1, &triangle.p2);
    if (triangle.p0.y > triangle.p1.y)
        Vector2_swap(&triangle.p0, &triangle.p1);
}

static void barycentric_rasterization(Renderer* self, Triangle triangle,
                                      u32 color) {}

static VertexOut* project_to_screen(const Renderer* self, const Camera* camera,
                                    const Mesh* mesh,
                                    const Transform* transform) {
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
    Matrix4 projection_matrix = Matrix4_perspective_projection(
        fov_y, self->width / (f32)self->height, near, far);

    // Get mesh members
    Vector3* mesh_vertices = Mesh_get_vertices(mesh);
    u32 vertex_count = Mesh_get_vertex_count(mesh);

    VertexOut* projected_vertices = calloc(vertex_count, sizeof(VertexOut));
    if (!projected_vertices) {
        fprintf(stderr, "project_vertices: "
                        "allocation failed\n");
        return NULL;
    }

    for (u32 i = 0; i < vertex_count; i++) {
        Vector4 local = Vector4_from_vector3(mesh_vertices[i]);
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
        projected_vertices[i].ndc.x = clip.x / clip.w;
        projected_vertices[i].ndc.y = clip.y / clip.w;
        projected_vertices[i].ndc.z = clip.z / clip.w;
        // Normalized Device Coordinates
        // (NDC) -> Screen coordinates
        projected_vertices[i].screen =
            viewport_transform(self, projected_vertices[i].ndc);
    }
    return projected_vertices;
}

static Vector2 viewport_transform(const Renderer* self, Vector3 v) {
    return (Vector2){(v.x + 1.0f) * self->width * 0.5f,
                     (1.0f - v.y) * self->height * 0.5f};
}

static void rasterize_points(Renderer* self, u32 vertex_count,
                             const VertexOut* projected_vertices) {
    for (u32 i = 0; i < vertex_count; i++) {
        Renderer_draw_pixel(self, projected_vertices[i].screen.x,
                            projected_vertices[i].screen.y, COLOR_WHITE);
    }
}

static void rasterize_wireframe(Renderer* self, const Mesh* mesh,
                                const VertexOut* projected_vertices) {
    u32* mesh_indices = Mesh_get_indices(mesh);
    u32 index_count = Mesh_get_index_count(mesh);
    for (u32 i = 0; i < index_count; i += 3) {
        Vector2 a = projected_vertices[mesh_indices[i]].screen;
        Vector2 b = projected_vertices[mesh_indices[i + 1]].screen;
        Vector2 c = projected_vertices[mesh_indices[i + 2]].screen;
        draw_triangle_wireframe(self, (Triangle){a, b, c}, COLOR_WHITE);
    }
}

static void rasterize_solid(Renderer* self, const Mesh* mesh,
                            const VertexOut* projected_vertices) {
    u32* mesh_indices = Mesh_get_indices(mesh);
    u32 index_count = Mesh_get_index_count(mesh);
    u32 colors[] = {COLOR_WHITE,  COLOR_BLUE, COLOR_CYAN, COLOR_MAGENTA,
                    COLOR_YELLOW, COLOR_RED,  COLOR_GREEN};

    for (u32 i = 0; i < index_count; i += 3) {
        Vector2 a = projected_vertices[mesh_indices[i]].screen;
        Vector2 b = projected_vertices[mesh_indices[i + 1]].screen;
        Vector2 c = projected_vertices[mesh_indices[i + 2]].screen;
        draw_triangle_solid(self, (Triangle){a, b, c}, colors[i % 7]);
    }
}
