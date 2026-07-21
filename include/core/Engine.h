#ifndef ENGINE_H
#define ENGINE_H

#include "geometry/Triangle.h"
#include "graphics/Camera.h"
#include "input/Input.h"
#include "math/Vector2.h"
#include "scene/Object3D.h"
#include "types.h"
#include <stdbool.h>

typedef struct Engine Engine;

typedef struct EngineCallbacks {
        void (*update)(Engine*);
        void (*draw)(Engine*);
} EngineCallbacks;

bool Engine_create(Engine** engine);
void Engine_destroy(Engine** engine);
void Engine_run(Engine* e, const EngineCallbacks* callbacks);
Input* Engine_get_input(Engine* e);
void Engine_draw_pixel(Engine* e, int x, int y, u32 color);
void Engine_draw_line(Engine* e, Vector2 start, Vector2 end, u32 color);
void Engine_draw_triangle(Engine* e, Triangle triangle, u32 color);
void Engine_draw_triangle_wireframe(Engine* e, Triangle triangle, u32 color);
void Engine_draw_object3D(const Engine* e, const Camera* camera,
                          const Object3D* obj);

#endif
