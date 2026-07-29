#ifndef ENGINE_H
#define ENGINE_H

#include "Color.h"
#include "geometry/Triangle.h"
#include "graphics/Camera.h"
#include "graphics/RasterMode.h"
#include "input/Input.h"
#include "math/Vector2.h"
#include "scene/Object3D.h"
#include "types.h"
#include <stdbool.h>

typedef struct Engine Engine;

typedef struct EngineCallbacks {
        void (*update)(Engine*, f32 dt);
        void (*draw)(Engine*);
} EngineCallbacks;

bool Engine_create(Engine** engine);
void Engine_destroy(Engine** engine);
void Engine_set_raster_mode(Engine* self, RasterMode raster_mode);
RasterMode Engine_get_raster_mode(const Engine* self);
Input* Engine_get_input(Engine* e);

void Engine_run(Engine* e, const EngineCallbacks* callbacks);

void Engine_draw_pixel(const Engine* self, i32 x, i32 y, Color color);
void Engine_draw_line(const Engine* self, Vector2 a, Vector2 b, Color color);
// void Engine_draw_triangle(Engine* e, Triangle triangle, Color color);
// void Engine_draw_object3D(const Engine* e, const Camera* camera,
//                           const Object3D* obj);

#endif
