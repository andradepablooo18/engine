#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "math/Matrix4.h"
#include "math/Vector3.h"

typedef struct Transform {
        Vector3 position;
        Vector3 scale;
        Vector3 rotation;
} Transform;

Transform Transform_identity(void);
Matrix4 Transform_get_matrix(const Transform* transform);
void Transform_translate(Transform* transform, Vector3 delta);
void Transform_scale(Transform* transform, Vector3 delta);
void Transform_rotate(Transform* transform, Vector3 delta);

#endif
