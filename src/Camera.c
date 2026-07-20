#include "Camera.h"
#include "math/common.h"
#include <stdio.h>

// void Camera_set_position(Camera* camera, Vector3 position) {
//     if (!camera) {
//         fprintf(stderr, "Camera_set_position: Camera* is NULL\n");
//         return;
//     }
//     camera->position = position;
// }

/* Camera_create: creates a camera with default values in transform (i.e
 * position, scale, rotation) and fov, near plane and far plane */
void Camera_create(Camera* self) {
    if (!self) {
        fprintf(stderr, "Camera_create: Camera* is NULL\n");
        return;
    }

    // Camera's position, scale and rotation
    self->transform.position = (Vector3){0, 0, 0};
    self->transform.scale = (Vector3){0, 0, 0};
    self->transform.rotation = (Vector3){0, 0, 0};

    // Camera's configs
    self->fov_y = PI / 3.0f; // 60°
    self->near = 0.1f;
    self->far = 100.0f;
}

/* Camera_get_view_matrix: returns a view matrix that when applied to an object
 * brings it from world space to view space */
Matrix4 Camera_get_view_matrix(const Camera* self) {
    if (!self) {
        fprintf(stderr, "Camera_get_view_matrix: Camera* is NULL\n");
        return (Matrix4){0};
    }
    return Matrix4_translation(-self->transform.position.x,
                               -self->transform.position.y,
                               -self->transform.position.z);
}
