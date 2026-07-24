#ifndef CAMERA_H
#define CAMERA_H

#include "math/Matrix4.h"
#include <stdbool.h>

typedef struct Camera Camera;

bool Camera_create(Camera** self);
void Camera_destroy(Camera** self);
// void Camera_set_position(Camera* camera, Vector3 position);
f32 Camera_get_fov(const Camera* self);
f32 Camera_get_near(const Camera* self);
f32 Camera_get_far(const Camera* self);
Matrix4 Camera_get_view_matrix(const Camera* self);

void Camera_translate(Camera* camera, Vector3 delta);

#endif
