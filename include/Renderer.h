#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"
#include "types.h"
#include <SDL3/SDL.h>

typedef struct Renderer Renderer;

bool Renderer_create(Renderer** self, Window* window);
void Renderer_destroy(Renderer** self);
void Renderer_clear(Renderer* self, u32 color);
void Renderer_present(Renderer* self);
void Renderer_draw_pixel(Renderer* self, int x, int y, u32 color);
// Renderer_draw_line
// Renderer_draw_triangle

#endif
