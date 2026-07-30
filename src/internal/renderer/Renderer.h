#ifndef RENDERER_H
#define RENDERER_H

#include "core/Color.h"
#include "core/types.h"
#include "geometry/Triangle.h"
#include "graphics/Camera.h"
#include "graphics/RasterMode.h"
#include "internal/window/Window.h"
#include "math/Vector2.h"
#include "scene/Object3D.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct Renderer Renderer;

bool Renderer_create(Renderer** self, Window* window, RasterMode raster_mode);
void Renderer_destroy(Renderer** self);
void Renderer_set_raster_mode(Renderer* self, RasterMode raster_mode);
RasterMode Renderer_get_raster_mode(const Renderer* self);

void Renderer_clear(Renderer* self, Color color);
void Renderer_present(Renderer* self);

void Renderer_draw_pixel(i32 x, i32 y, Color color, const Renderer* self);
void Renderer_draw_line(Vector2 a, Vector2 b, Color color,
                        const Renderer* self);
void Renderer_draw_triangle(Triangle triangle, const Renderer* self);
// void Renderer_draw_object3D(Renderer* self, const Camera* camera,
//                             const Object3D* obj);

#endif
