#ifndef SCENE_H
#define SCENE_H

#include "graphics/Camera.h"
#include "scene/Object3D.h"

typedef struct Scene {
        Camera* camera;
        Object3D** objects;
        u32 object_count;
        u32 object_capacity;
} Scene;

#endif
