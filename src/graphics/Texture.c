#include "graphics/Texture.h"
#include "math/common.h"
#include <SDL3_image/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Texture {
        Color* pixels;
        u32 width;
        u32 height;
};

// bool Texture_create(Texture** self, u32 width, u32 height) {
//     assert(self);
//     *self = calloc(1, sizeof(Texture));
//     if (!*self) {
//         fprintf(stderr, "Texture_create: Texture allocation failed\n");
//         return false;
//     }
//
//     Texture* const s = *self;
//     s->width = width;
//     s->height = height;
//     s->pixels = calloc(s->width * s->height, sizeof(Color));
//     if (!s->pixels) {
//         fprintf(stderr, "Texture_create: Texture's pixels allocation
//         failed\n"); return false;
//     }
//
//     return true;
// }

bool Texture_create(Texture** self, const char* path) {
    assert(self);

    *self = calloc(1, sizeof(Texture));
    if (!*self) {
        fprintf(stderr, "Texture_create: Texture allocation failed\n");
        return false;
    }

    SDL_Surface* loaded = IMG_Load(path);
    if (!loaded) {
        fprintf(stderr, "IMG_Load failed: %s\n", SDL_GetError());
        Texture_destroy(self);
        return false;
    }

    SDL_Surface* surface = SDL_ConvertSurface(loaded, SDL_PIXELFORMAT_RGBA8888);
    SDL_DestroySurface(loaded);
    if (!surface) {
        fprintf(stderr, "SDL_ConvertSurface failed: %s\n", SDL_GetError());
        Texture_destroy(self);
        return false;
    }

    /* Just for making code more readable */
    Texture* t = *self;

    t->width = surface->w;
    t->height = surface->h;

    size_t pixel_count = t->width * t->height;

    t->pixels = calloc(pixel_count, sizeof(Color));
    if (!t->pixels) {
        fprintf(stderr, "Texture_create: Texture's pixels allocation failed\n");
        SDL_DestroySurface(surface);
        Texture_destroy(self);
        return false;
    }

    memcpy(t->pixels, surface->pixels, pixel_count * sizeof(Color));

    SDL_DestroySurface(surface);

    return true;
}

bool Texture_create_checker(Texture** self) {
    assert(self);
    *self = calloc(1, sizeof(Texture));
    if (!*self) {
        fprintf(stderr, "Texture_create_checker: Texture allocation failed\n");
        return false;
    }

    Texture* const s = *self;
    s->width = 8;
    s->height = 8;
    s->pixels = calloc(s->width * s->height, sizeof(Color));
    if (!s->pixels) {
        fprintf(stderr,
                "Texture_create_checker: Texture's pixel allocation failed\n");
        return false;
    }

    for (u32 y = 0; y < s->height; y++) {
        for (u32 x = 0; x < s->width; x++) {
            bool white = ((x + y) % 2) == 0;
            s->pixels[y * s->width + x] = white ? COLOR_WHITE : COLOR_BLACK;
        }
    }

    return true;
}

void Texture_destroy(Texture** self) {
    assert(self);
    if (!*self)
        return;

    Texture* t = *self;

    free(t->pixels);
    free(t);

    *self = NULL;
}

Color Texture_sample(const Texture* self, f32 u, f32 v) {
    assert(self);
    i32 x = (i32)(u * self->width);
    i32 y = (i32)(v * self->height);
    x = Math_clamp_i32(x, 0, self->width - 1);
    y = Math_clamp_i32(y, 0, self->height - 1);
    return self->pixels[y * self->width + x];
}
