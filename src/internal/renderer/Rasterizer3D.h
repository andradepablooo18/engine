#ifndef RASTERIZER3D_H
#define RASTERIZER3D_H

#include "core/Color.h"
#include "core/types.h"
#include "graphics/Camera.h"
#include "scene/Object3D.h"

void Rasterizer3D_draw_object3D_solid(const Camera* camera, const Object3D* obj,
                                      Color* frame_buffer, f32* depth_buffer,
                                      i32 width, i32 height);

#endif
