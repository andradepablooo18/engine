#include "graphics/Texture.h"
#include "math/common.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Texture {
        Color* pixels;
        u32 width;
        u32 height;
};

bool Texture_create(Texture** self, u32 width, u32 height) {
    assert(self);
    *self = calloc(1, sizeof(Texture));
    if (!*self) {
        fprintf(stderr, "Texture_create: Texture allocation failed\n");
        return false;
    }

    Texture* const s = *self;
    s->width = width;
    s->height = height;
    s->pixels = calloc(s->width * s->height, sizeof(Color));
    if (!s->pixels) {
        fprintf(stderr, "Texture_create: Texture's pixels allocation failed\n");
        return false;
    }

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
    free(*self);
    *self = NULL;
}

Color Texture_sample(const Texture* self, f32 u, f32 v) {
    assert(self);
    u = Math_clamp(u, 0.0f, 0.9999f);
    v = Math_clamp(v, 0.0f, 0.9999f);
    u32 x = (u32)(u * (self->width - 1));
    u32 y = (u32)(v * (self->height - 1));
    return self->pixels[y * self->width + x];
}
