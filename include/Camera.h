#ifndef CAMERA_H
#define CAMERA_H

#include "Transform.h"
#include "math/Matrix4.h"
#include "math/Vector3.h"

typedef struct Camera {
        Transform transform;
        f32 fov_y; // in radians
        f32 near;
        f32 far;
} Camera;

void Camera_create(Camera* self);
// void Camera_set_position(Camera* camera, Vector3 position);
Matrix4 Camera_get_view_matrix(const Camera* self);

#endif
