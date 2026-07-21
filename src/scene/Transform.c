#include "scene/Transform.h"
#include <stdio.h>

/* Transform_identity: returns a transform with default values in postion, scale
 * and rotation */
Transform Transform_identity(void) {
    return (Transform){
        .position = {0, 0, 0}, .scale = {1, 1, 1}, .rotation = {0, 0, 0}};
}

/* Transform_get_matrix: returns a model matrix that when applied to an object
 * brings it from object space to world space */
Matrix4 Transform_get_matrix(const Transform* transform) {
    if (!transform) {
        fprintf(stderr, "Transform_get_matrix: Transform* is NULL\n");
        return (Matrix4){0};
    }

    Matrix4 translation = Matrix4_translation(
        transform->position.x, transform->position.y, transform->position.z);

    Matrix4 scale = Matrix4_scale(transform->scale.x, transform->scale.y,
                                  transform->scale.z);

    Matrix4 rotation_x = Matrix4_rotation_x(transform->rotation.x);
    Matrix4 rotation_y = Matrix4_rotation_y(transform->rotation.y);
    Matrix4 rotation_z = Matrix4_rotation_z(transform->rotation.z);

    Matrix4 rotation =
        Matrix4_multiply(rotation_x, Matrix4_multiply(rotation_y, rotation_z));

    return Matrix4_multiply(translation, Matrix4_multiply(rotation, scale));
}
