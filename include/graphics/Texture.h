#ifndef TEXTURE_H
#define TEXTURE_H

#include "core/Color.h"
#include "core/types.h"
#include <stdbool.h>

typedef struct Texture Texture;

bool Texture_create(Texture** self, u32 width, u32 height);
bool Texture_create_checker(Texture** self);
void Texture_destroy(Texture** self);

Color Texture_sample(const Texture* self, f32 u, f32 v);

#endif
