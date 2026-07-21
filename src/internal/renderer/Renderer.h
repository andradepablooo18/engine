#ifndef RENDERER_H
#define RENDERER_H

#include "core/types.h"
#include "geometry/Triangle.h"
#include "graphics/Camera.h"
#include "internal/window/Window.h"
#include "math/Vector2.h"
#include "scene/Object3D.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct Renderer Renderer;

bool Renderer_create(Renderer** self, Window* window);
void Renderer_destroy(Renderer** self);

void Renderer_clear(Renderer* self, u32 color);
void Renderer_present(Renderer* self);

void Renderer_draw_pixel(Renderer* self, i32 x, i32 y, u32 color);

void Renderer_draw_line(Renderer* self, Vector2 start, Vector2 end, u32 color);

void Renderer_draw_triangle(Renderer* self, Triangle triangle, u32 color);
void Renderer_draw_triangle_wireframe(Renderer* self, Triangle triangle,
                                      u32 color);

void Renderer_draw_object3D(Renderer* self, const Camera* camera,
                            const Object3D* obj);

#endif
