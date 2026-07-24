#include "graphics/Camera.h"
#include "math/Matrix4.h"
#include "math/common.h"
#include "scene/Transform.h"
#include <stdio.h>
#include <stdlib.h>

struct Camera {
        Transform transform;
        f32 fov_y; // in radians
        f32 near;
        f32 far;
};

// void Camera_set_position(Camera* camera, Vector3 position) {
//     if (!camera) {
//         fprintf(stderr, "Camera_set_position: Camera* is NULL\n");
//         return;
//     }
//     camera->position = position;
// }
//

/* Camera_create: creates a camera with default values in transform (i.e
 * position, scale, rotation), fov, near plane and far plane */
bool Camera_create(Camera** self) {
    if (!self) {
        fprintf(stderr, "Camera_create: Camera** is NULL\n");
        return false;
    }
    *self = calloc(1, sizeof(Camera));
    if (!*self) {
        fprintf(stderr, "Camera_create: allocation failed\n");
        return false;
    }
    Camera* s = *self;
    s->transform.position = (Vector3){0, 0, 0};
    s->transform.scale = (Vector3){0, 0, 0};
    s->transform.rotation = (Vector3){0, 0, 0};
    s->fov_y = PI / 3.0f; // 60°
    s->near = 0.1f;
    s->far = 100.0f;
    return true;
}

void Camera_destroy(Camera** self) {
    if (!self || !*self)
        return;
    free(*self);
    *self = NULL;
}

f32 Camera_get_fov(const Camera* self) {
    if (!self) {
        fprintf(stderr, "Camera_get_fov: Camera* is NULL\n");
        return -1.0f;
    }
    return self->fov_y;
}

f32 Camera_get_near(const Camera* self) {
    if (!self) {
        fprintf(stderr, "Camera_get_near: Camera* is NULL\n");
        return -1.0f;
    }
    return self->near;
}

f32 Camera_get_far(const Camera* self) {
    if (!self) {
        fprintf(stderr, "Camera_get_far: Camera* is NULL\n");
        return -1.0f;
    }
    return self->far;
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

void Camera_translate(Camera* camera, Vector3 delta) {
    Transform_translate(&camera->transform, delta);
}
