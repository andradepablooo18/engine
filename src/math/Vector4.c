#include "math/Vector4.h"

Vector4 Vector4_from_vector3(Vector3 v) {
    return (Vector4){.x = v.x, .y = v.y, .z = v.z, .w = 1.0f};
}
