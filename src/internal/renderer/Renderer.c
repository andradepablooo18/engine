#include "Renderer.h"
#include "types.h"
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

/*
 ************************************************
 ************************************************
 *
 * STATIC HELPERS
 *
 ************************************************
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
