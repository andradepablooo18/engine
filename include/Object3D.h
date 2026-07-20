#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Mesh.h"
#include "Transform.h"

typedef struct Object3D {
        Mesh* mesh;
        Transform transform;
} Object3D;

void Object3D_create_cube(Object3D* obj);
void Object3D_destroy(Object3D* obj);

#endif
