#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Mesh.h"
#include "Transform.h"

typedef struct Object3D {
        Mesh* mesh;
        Transform transform;
} Object3D;

#endif
