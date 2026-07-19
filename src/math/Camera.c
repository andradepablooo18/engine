#include "math/Camera.h"
#include <_stdio.h>
#include <stdio.h>

// void Camera_set_position(Camera* camera, Vector3 position) {
//     if (!camera) {
//         fprintf(stderr, "Camera_set_position: Camera* is NULL\n");
//         return;
//     }
//     camera->position = position;
// }

Matrix4 Camera_get_view_matrix(const Camera* camera) {
    if (!camera) {
        fprintf(stderr, "Camera_get_view_matrix: Camera* is NULL\n");
        return (Matrix4){0};
    }
    return Matrix4_translation(-camera->position.x, -camera->position.y,
                               -camera->position.z);
}
