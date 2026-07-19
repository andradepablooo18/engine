#ifndef CAMERA_H
#define CAMERA_H

#include "math/Matrix4.h"
#include "math/Vector3.h"

typedef struct Camera {
        Vector3 position;
} Camera;

// Camera Camera_create(void);
// void Camera_set_position(Camera* camera, Vector3 position);
Matrix4 Camera_get_view_matrix(const Camera* camera);

#endif
