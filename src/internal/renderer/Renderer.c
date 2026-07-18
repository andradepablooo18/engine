#include "Renderer.h"
#include "math/common.h"
#include "types.h"
#include <_string.h>
#include <math.h>
#include <stdlib.h>

struct Renderer {
        SDL_Renderer* handle;
        SDL_Texture* color_buffer_texture;
        u32* color_buffer;
        // f32* depth_buffer;
        i32 width;
        i32 height;
};

static void Renderer_draw_line_DDA(Renderer* self, Vector2 start, Vector2 end,
                                   u32 color);
static void Renderer_draw_line_bresenham(Renderer* self, Vector2 start,
                                         Vector2 end, u32 color);
static void Renderer_draw_triangle_scanline(Renderer* self, Triangle triangle,
                                            u32 color);
static inline void Vector2_swap(Vector2* a, Vector2* b);
static i32* Interpolate(Vector2 start, Vector2 end);
static void sum_arrays(i32* dest, i32 d_length, const i32* src1, i32 s1_length,
                       const i32* src2, i32 s2_length);

bool Renderer_create(Renderer** self, Window* window) {
    if (!self) {
        fprintf(stderr,
                "Renderer_create: invalid argument (Renderer** is NULL)\n");
        return false;
    }
    if (!window) {
        fprintf(stderr,
                "Renderer_create: invalid argument (Window* is NULL)\n");
        return false;
    }

    *self = calloc(1, sizeof(Renderer));
    if (!*self) {
        fprintf(stderr, "Renderer_create: allocation failed\n");
        return false;
    }

    Renderer* const s = *self;

    s->handle = SDL_CreateRenderer(Window_get_handle(window), NULL);
    if (!s->handle) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
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
        fprintf(stderr, "Renderer_create failed while allocating memory for "
                        "color_buffer\n");
        Renderer_destroy(self);
        return false;
    }

    return true;
}

void Renderer_destroy(Renderer** self) {
    if (!self || !*self)
        return;

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

void Renderer_clear(Renderer* self, u32 color) {
    if (!self) {
        fprintf(stderr, "Renderer_clear: argument is NULL\n");
        return;
    }
    const size_t size = (size_t)self->width * self->height;
    for (size_t i = 0; i < size; i++) {
        self->color_buffer[i] = color;
    }
}

void Renderer_present(Renderer* self) {
    if (!self) {
        fprintf(stderr, "Renderer_present: argument is NULL\n");
        return;
    }
    SDL_UpdateTexture(self->color_buffer_texture, NULL, self->color_buffer,
                      self->width * sizeof(u32));
    SDL_RenderTexture(self->handle, self->color_buffer_texture, NULL, NULL);
    SDL_RenderPresent(self->handle);
}

void Renderer_draw_pixel(Renderer* self, i32 x, i32 y, u32 color) {
    if (!self) {
        fprintf(stderr, "Renderer_draw_pixel: argument is NULL\n");
        return;
    }
    if (x < 0 || x >= self->width || y < 0 || y >= self->height)
        return;
    self->color_buffer[y * self->width + x] = color;
}

void Renderer_draw_line(Renderer* self, Vector2 start, Vector2 end, u32 color) {
    if (!self) {
        fprintf(stderr, "Renderer_draw_line: argument is NULL\n");
        return;
    }
    // Renderer_draw_line_DDA(self, x1, y1, x2, y2, color);
    Renderer_draw_line_bresenham(self, start, end, color);
}

void Renderer_draw_triangle_wireframe(Renderer* self, Triangle triangle,
                                      u32 color) {
    if (!self) {
        fprintf(stderr, "Renderer_draw_triangle_wireframe: argument is NULL\n");
        return;
    }
    Renderer_draw_line(self, triangle.p0, triangle.p1, color);
    Renderer_draw_line(self, triangle.p1, triangle.p2, color);
    Renderer_draw_line(self, triangle.p0, triangle.p2, color);
}

void Renderer_draw_triangle(Renderer* self, Triangle triangle, u32 color) {
    if (!self) {
        fprintf(stderr, "Renderer_draw_triangle: argument is NULL\n");
        return;
    }
    Renderer_draw_triangle_scanline(self, triangle, color);
}

/*
 ************************************************
 ************************************************
 *
 * STATIC HELPERS
 *
 ************************************************
 ************************************************
 */

/*
 ************************************************
 *
 * DRAW LINE HELPERS (based on which algorithm it is used)
 *
 ************************************************
 */

static void Renderer_draw_line_DDA(Renderer* self, Vector2 start, Vector2 end,
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

static void Renderer_draw_line_bresenham(Renderer* self, Vector2 start,
                                         Vector2 end, u32 color) {
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

/*
 ************************************************
 *
 * DRAW FILLED TRIANGLE HELPERS (based on which algorithm it is used)
 *
 ************************************************
 */

// Draw filled triangle using scanline rasterization algorithm
static void Renderer_draw_triangle_scanline(Renderer* self, Triangle triangle,
                                            u32 color) {
    // Sort triangle vertices based on ints y coordinate from smallest to
    // biggest so that p0.y <= p1.y <= p2.y
    if (triangle.p0.y > triangle.p1.y)
        Vector2_swap(&triangle.p0, &triangle.p1);
    if (triangle.p1.y > triangle.p2.y)
        Vector2_swap(&triangle.p1, &triangle.p2);
    if (triangle.p0.y > triangle.p1.y)
        Vector2_swap(&triangle.p0, &triangle.p1);

    // For each y there must be a range in x to be filled
    f32 x_left;
    f32 x_right;

    i32 x0;
    i32 x1;
    i32 y0;
    i32 y1;

    // Check for degenerated triangles
    if ((triangle.p2.y - triangle.p0.y) < MATH_EPSILON) {
        if (triangle.p0.x < triangle.p2.x) {
            x_left = triangle.p0.x;
            x_right = triangle.p2.x;
        } else {
            x_left = triangle.p2.x;
            x_right = triangle.p0.x;
        }
        x0 = (i32)roundf(x_left);
        x1 = (i32)roundf(x_right);
        y0 = (i32)roundf(triangle.p0.y);
        for (i32 x = x0; x <= x1; x++) {
            Renderer_draw_pixel(self, x, y0, color);
        }
        return;
    }

    // For each line, calculate each variation in x per y

    // First line (the longest)
    f32 dx02 = triangle.p2.x - triangle.p0.x;
    f32 dy02 = triangle.p2.y - triangle.p0.y;
    f32 x_step02 = dx02 / dy02;

    f32 dx01 = triangle.p1.x - triangle.p0.x;
    f32 dy01 = triangle.p1.y - triangle.p0.y;
    f32 x_step01 = 0.0f;
    if (dy01 > MATH_EPSILON) {
        x_step01 = dx01 / dy01;
    }

    f32 dx12 = triangle.p2.x - triangle.p1.x;
    f32 dy12 = triangle.p2.y - triangle.p1.y;
    f32 x_step12 = 0.0f;
    if (dy12 > MATH_EPSILON) {
        x_step12 = dx12 / dy12;
    }

    // For first half:

    // Select which side is left and which one is right
    // Then set how much it is going to change x_left and x_rigth in each
    // iteration
    f32 x_middle = triangle.p0.x + x_step02 * (triangle.p1.y - triangle.p0.y);
    f32 x_left_step;
    f32 x_right_step;
    if (x_middle < triangle.p1.x) {
        x_left_step = x_step02;
        x_right_step = x_step01;
    } else {
        x_left_step = x_step01;
        x_right_step = x_step02;
    }
    // Set start point and end point in y between p0 and p1
    y0 = (i32)ceilf(triangle.p0.y);
    y1 = (i32)floorf(triangle.p1.y);
    // Set initial values for these as they start in the same highest point
    x_left = triangle.p0.x;
    x_right = triangle.p0.x;

    // Draw pixels horizontally from x_left to x_right per y
    for (i32 y = y0; y < y1; y++) {
        // Set start point and end point in x between p0 and p1
        i32 x0 = (i32)roundf(x_left);
        i32 x1 = (i32)roundf(x_right);

        for (i32 x = x0; x <= x1; x++) {
            Renderer_draw_pixel(self, x, y, color);
        }

        x_left += x_left_step;
        x_right += x_right_step;
    }

    // Do almost same for second half:

    // Change x_step in one of the sides as we are now computing the remaining
    // side
    if (x_middle < triangle.p1.x) {
        x_right_step = x_step12;
        x_left = x_middle;
        x_right = triangle.p1.x;
    } else {
        x_left_step = x_step12;
        x_left = triangle.p1.x;
        x_right = x_middle;
    }
    // Set start point and end point in y between p1 and p2
    // we did p1.y in last iteration so add 1
    y0 = (i32)ceilf(triangle.p1.y);
    y1 = (i32)floorf(triangle.p2.y);

    for (i32 y = y0; y <= y1; y++) {
        // Set starting points in x between p0 and p1
        i32 x0 = (i32)roundf(x_left);
        i32 x1 = (i32)roundf(x_right);

        for (i32 x = x0; x <= x1; x++) {
            Renderer_draw_pixel(self, x, y, color);
        }

        x_left += x_left_step;
        x_right += x_right_step;
    }
}

static inline void Vector2_swap(Vector2* a, Vector2* b) {
    Vector2 temp = *a;
    *a = *b;
    *b = temp;
}

// Interpolates all x values between start and end
static i32* Interpolate(Vector2 start, Vector2 end) {
    i32 x1 = (i32)roundf(start.x);
    i32 y1 = (i32)roundf(start.y);

    i32 x2 = (i32)roundf(end.x);
    i32 y2 = (i32)roundf(end.y);

    i32 dx = x2 - x1;
    i32 dy = y2 - y1;

    i32* x_values = calloc(abs(dy) + 1, sizeof(i32));
    if (!x_values)
        return NULL;

    if (dy <= 0) {
        x_values[0] = start.x;
        return x_values;
    }

    f32 slope = dx / (f32)dy;

    f32 x = x1;

    for (i32 y = y1; y <= y2; y++) {
        x_values[y - y1] = (i32)roundf(x);
        x += slope;
    }

    return x_values;
}

static void sum_arrays(i32* dest, i32 d_length, const i32* src1, i32 s1_length,
                       const i32* src2, i32 s2_length) {
    if (!dest || !src1 || !src2) {
        fprintf(stderr, "sum_arrays: some i32 array is NULL\n");
        return;
    }
    if (d_length < s1_length + s2_length) {
        fprintf(stderr, "sum_arrays: destination array is too small\n");
        return;
    }

    for (i32 i = 0; i < s1_length; i++) {
        dest[i] = src1[i];
    }
    for (i32 i = 0; i < s2_length; i++) {
        dest[i + s1_length] = src2[i];
    }
}
