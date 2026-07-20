#include "Object3D.h"
#include <stdio.h>
#include <stdlib.h>

void Object3D_create_cube(Object3D* obj) {
    obj->mesh = calloc(1, sizeof(Mesh));
    if (!obj->mesh) {
        fprintf(stderr, "Object3D_create_cube: allocation failed\n");
    }
    Mesh_create_cube(obj->mesh);
    obj->transform = Transform_identity();
    obj->transform.position.z = -10;
}

void Object3D_destroy(Object3D* obj) { Mesh_destroy(&obj->mesh); }
