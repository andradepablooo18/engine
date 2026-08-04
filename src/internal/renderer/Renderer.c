#include "Renderer.h"
#include "Rasterizer2D.h"
#include "Rasterizer3D.h"
#include "graphics/Camera.h"
#include "graphics/Mesh.h"
#include "graphics/RasterMode.h"
#include "graphics/VertexOut.h"
#include "math/Vector3.h"
#include "math/common.h"
#include "scene/Object3D.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

struct Renderer {
        SDL_Renderer* handle;
        SDL_Texture* color_buffer_texture;
        Color* color_buffer;
        f32* depth_buffer;
        i32 width;
        i32 height;
        RasterMode raster_mode;
};

/*
 *************************************************************************
 *************************************************************************
 *
 * Renderer_draw_triangle HELPERS (based on rasterization mode)
 *
 *************************************************************************
 *************************************************************************
 */

// static void draw_triangle_points(Renderer* self, Vector2 p0, Vector2 p1,
//                                  Vector2 p2, Color color);
// static void draw_triangle_wireframe(Renderer* self, Vector2 p0, Vector2 p1,
//                                     Vector2 p2, Color color);
// static void draw_triangle_solid(Renderer* self, Vector2 p0, Vector2 p1,
//                                 Vector2 p2, Color color);

/*
 *************************************************************************
 *************************************************************************
 *
 * draw_triangle_solid HELPERS (based on which algorithm it is used)
 *
 *************************************************************************
 *************************************************************************
 */

// static void scanline_rasterization(Renderer* self, Triangle triangle,
//                                    Color color);
//
// static void barycentric_rasterization(Renderer* self, Vector2 a, Vector2 b,
//                                       Vector2 c, Color color);

/*
 *************************************************************************
 *************************************************************************
 *
 * barycentric_rasterization HELPERS
 *
 *************************************************************************
 *************************************************************************
 */

// bool is_top_left(const Vector2* a, const Vector2* b);
// static f32 edge(const Vector2* a, const Vector2* b, const Vector2* p);

/*
 *************************************************************************
 *************************************************************************
 *
 * Renderer_draw_object3D HELPERS
 *
 *************************************************************************
 *************************************************************************
 */

// static VertexOut* project_to_screen(const Renderer* self, const Camera*
// camera,
//                                     const Mesh* mesh,
//                                     const Transform* transform);
// static VertexOut viewport_transform(const Renderer* self, const Vector3*
// ndc);
//
/*
 *************************************************************************
 *************************************************************************
 *
 * Renderer_draw_object3D HELPERS (based on rasterization mode)
 *
 *************************************************************************
 *************************************************************************
 */

// static void draw_object3D_vertices(Renderer* self,
//                                    const VertexOut* projected_vertices,
//                                    u32 vertex_count);
// static void draw_mesh_wireframe(Renderer* self, const Mesh* mesh,
//                                 const VertexOut* projected_vertices);
// static void draw_mesh_solid(Renderer* self, const Mesh* mesh,
//                             VertexOut* projected_vertices);

/*
 *************************************************************************
 *************************************************************************
 *
 * draw_mesh_solid HELPERS (based on mode)
 *
 *************************************************************************
 *************************************************************************
 */

// static void fill_vertex_out_triangle(Renderer* self, VertexOut* a_out,
//                                      VertexOut* b_out, VertexOut* c_out);
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
    SDL_SetRenderVSync(s->handle, 1);

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
    SDL_SetTextureScaleMode(s->color_buffer_texture, SDL_SCALEMODE_NEAREST);

    s->color_buffer = calloc(width * height, sizeof(Color));
    if (!s->color_buffer) {
        fprintf(stderr, "Renderer_create failed while "
                        "allocating memory for "
                        "color_buffer\n");
        Renderer_destroy(self);
        return false;
    }

    s->depth_buffer = calloc(width * height, sizeof(f32));
    if (!s->color_buffer) {
        fprintf(stderr, "Renderer_create failed while "
                        "allocating memory for "
                        "depth_buffer\n");
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
    if (s->depth_buffer) {
        free(s->depth_buffer);
        s->depth_buffer = NULL;
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

RasterMode Renderer_get_raster_mode(const Renderer* self) {
    return self->raster_mode;
}

void Renderer_clear(Renderer* self, Color color) {
    const size_t size = (size_t)self->width * self->height;
    for (size_t i = 0; i < size; i++) {
        self->color_buffer[i] = color;
        self->depth_buffer[i] = 1.0f;
    }
}

void Renderer_present(Renderer* self) {
    SDL_UpdateTexture(self->color_buffer_texture, NULL, self->color_buffer,
                      self->width * sizeof(Color));
    SDL_RenderTexture(self->handle, self->color_buffer_texture, NULL, NULL);
    SDL_RenderPresent(self->handle);
}

void Renderer_draw_pixel(const Renderer* self, i32 x, i32 y, Color color) {
    assert(self);
    Rasterizer2D_draw_pixel(x, y, color, self->color_buffer, self->width,
                            self->height);
}

void Renderer_draw_line(const Renderer* self, Vector2 a, Vector2 b,
                        Color color) {
    assert(self);
    Rasterizer2D_draw_line(a, b, color, self->color_buffer, self->width,
                           self->height);
}

void Renderer_draw_triangle(const Renderer* self, Triangle triangle) {
    assert(self);
    switch (self->raster_mode) {
        case RASTER_MODE_POINTS:
            // TODO:
            // draw_triangle_points(self, triangle.p0, triangle.p1, triangle.p2,
            //                      color);
            break;
        case RASTER_MODE_WIREFRAME:
            // TODO:
            // draw_triangle_wireframe(self, triangle.p0, triangle.p1,
            // triangle.p2,
            //                         color);
            break;
        case RASTER_MODE_SOLID:
            Rasterizer2D_draw_triangle_solid(triangle.p0, triangle.p1,
                                             triangle.p2, self->color_buffer,
                                             self->width, self->height);
            break;
        default:
            break;
    }
}

void Renderer_draw_object3D(const Renderer* self, const Camera* camera,
                            const Object3D* obj) {
    assert(self);
    switch (self->raster_mode) {
        case RASTER_MODE_POINTS:
            // TODO:
            // draw_object3D_vertices(self, projected_vertices,
            //                        Mesh_get_vertex_count(mesh));
            break;
        case RASTER_MODE_WIREFRAME:
            // TODO:
            // draw_mesh_wireframe(self, mesh, projected_vertices);
            break;
        case RASTER_MODE_SOLID:
            Rasterizer3D_draw_object3D_solid(camera, obj, self->color_buffer,
                                             self->depth_buffer, self->width,
                                             self->height);
            break;
        default:
            break;
    }
}

// static void draw_triangle_points(Renderer* self, Vector2 p0, Vector2 p1,
//                                  Vector2 p2, Color color) {
//     Renderer_draw_pixel(self, p0.x, p0.y, color);
//     Renderer_draw_pixel(self, p1.x, p1.y, color);
//     Renderer_draw_pixel(self, p2.x, p2.y, color);
// }
//
// static void draw_triangle_wireframe(Renderer* self, Vector2 p0, Vector2 p1,
//                                     Vector2 p2, Color color) {
//     Renderer_draw_line(self, p0, p1, color);
//     Renderer_draw_line(self, p1, p2, color);
//     Renderer_draw_line(self, p0, p2, color);
// }

// static void draw_triangle_solid(Renderer* self, Vector2 p0, Vector2 p1,
//                                 Vector2 p2, Color color) {
//     // scanline_rasterization(self, triangle, color);
//     barycentric_rasterization(self, p0, p1, p2, color);
// }
//
// static void scanline_rasterization(Renderer* self, Triangle triangle,
//                                    Color color) {}

// static void barycentric_rasterization(Renderer* self, Vector2 a, Vector2 b,
//                                       Vector2 c, Color color) {
//     Color colors[3] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE};
//     bool is_inside;
//
//     // Find bounding box with all the candidate pixels
//     i32 x_min = floor(Math_min(a.x, (Math_min(b.x, c.x))));
//     i32 y_min = floor(Math_min(a.y, Math_min(b.y, c.y)));
//     i32 x_max = ceil(Math_max(a.x, (Math_max(b.x, c.x))));
//     i32 y_max = ceil(Math_max(a.y, Math_max(b.y, c.y)));
//
//     // Compute the constant delta values that will be used for horizontal a
//     // vertical steps in order to avoid computing edge function each
//     // iteration
//     f32 delta_w0_col = b.y - c.y;
//     f32 delta_w1_col = c.y - a.y;
//     f32 delta_w2_col = a.y - b.y;
//
//     f32 delta_w0_row = c.x - b.x;
//     f32 delta_w1_row = a.x - c.x;
//     f32 delta_w2_row = b.x - a.x;
//
//     // Compute the area of the parallelogram
//     f32 area = edge(&a, &b, &c);
//
//     // Stick to top-left rule filling convention
//     f32 bias0 = is_top_left(&b, &c) ? 0 : -0.0001;
//     f32 bias1 = is_top_left(&c, &a) ? 0 : -0.0001;
//     f32 bias2 = is_top_left(&a, &b) ? 0 : -0.0001;
//
//     // Compute edge function to see if pixel is inside triangle
//     Vector2 p = {x_min + 0.5f, y_min + 0.5f};
//     f32 w0_row = edge(&b, &c, &p) + bias0;
//     f32 w1_row = edge(&c, &a, &p) + bias1;
//     f32 w2_row = edge(&a, &b, &p) + bias2;
//
//     // Loop all candidate pixels inside the bounding box
//     for (i32 y = y_min; y <= y_max; y++) {
//         f32 w0 = w0_row;
//         f32 w1 = w1_row;
//         f32 w2 = w2_row;
//         for (i32 x = x_min; x <= x_max; x++) {
//             if (area >= 0.0f) {
//                 is_inside = w0 >= 0 && w1 >= 0 && w2 >= 0;
//             } else {
//                 is_inside = w0 <= 0 && w1 <= 0 && w2 <= 0;
//             }
//             if (is_inside) {
//                 // Compute barycentric coordinates alpha, beta and gamma
//                 f32 alpha = w0 / area;
//                 f32 beta = w1 / area;
//                 f32 gamma = w2 / area;
//                 u8 r = alpha * Color_get_red(colors[0]) +
//                        beta * Color_get_red(colors[1]) +
//                        gamma * Color_get_red(colors[2]);
//                 u8 g = alpha * Color_get_green(colors[0]) +
//                        beta * Color_get_green(colors[1]) +
//                        gamma * Color_get_green(colors[2]);
//                 u8 b = alpha * Color_get_blue(colors[0]) +
//                        beta * Color_get_blue(colors[1]) +
//                        gamma * Color_get_blue(colors[2]);
//                 Renderer_draw_pixel(self, x, y, Color_create(r, g, b, 0xFF));
//             }
//             w0 += delta_w0_col;
//             w1 += delta_w1_col;
//             w2 += delta_w2_col;
//         }
//         w0_row += delta_w0_row;
//         w1_row += delta_w1_row;
//         w2_row += delta_w2_row;
//     }
// }
