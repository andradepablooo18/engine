#include "Renderer.h"
#include <stdlib.h>

struct Renderer {
        SDL_Renderer* handle;
        SDL_Texture* texture;
        u32* color_buffer;
        // float* depth_buffer;
        int width;
        int height;
};

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

    int width = Window_get_width(window);
    int height = Window_get_height(window);

    s->width = width;
    s->height = height;

    s->texture = SDL_CreateTexture(s->handle, SDL_PIXELFORMAT_RGBA8888,
                                   SDL_TEXTUREACCESS_STREAMING, width, height);
    if (!s->texture) {
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
    if (s->texture) {
        SDL_DestroyTexture(s->texture);
        s->texture = NULL;
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

    const size_t size = self->width * self->height;
    for (size_t i = 0; i < size; i++) {
        self->color_buffer[i] = color;
    }
}

void Renderer_present(Renderer* self) {
    if (!self) {
        fprintf(stderr, "Renderer_present: argument is NULL\n");
        return;
    }

    SDL_UpdateTexture(self->texture, NULL, self->color_buffer,
                      self->width * sizeof(u32));
    SDL_RenderTexture(self->handle, self->texture, NULL, NULL);
    SDL_RenderPresent(self->handle);
}

void Renderer_draw_pixel(Renderer* self, int x, int y, u32 color) {
    if (!self) {
        fprintf(stderr, "Renderer_draw_pixel: argument is NULL\n");
        return;
    }

    if (x < 0 || x > self->width || y < 0 || y > self->height)
        return;
    self->color_buffer[y * self->width + x] = color;
}
