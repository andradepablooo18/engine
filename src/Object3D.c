#include "Object3D.h"

void Object3D_create_cube(Object3D* obj) {
    Mesh_create_cube(obj->mesh);
    obj->transform = Transform_identity();
}

void Object3D_destroy(Object3D* obj) { Mesh_destroy(&obj->mesh); }
