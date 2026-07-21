#include "scene/Object3D.h"
#include <stdio.h>
#include <stdlib.h>

struct Object3D {
        Mesh* mesh;
        Transform transform;
};

bool Object3D_create(Object3D** self) {
    if (!self) {
        fprintf(stderr, "Object3D_create: Object3D** is NULL\n");
        return false;
    }
    *self = calloc(1, sizeof(Object3D));
    if (!*self) {
        fprintf(stderr, "Object3D_create: allocation failed\n");
        return false;
    }
    (*self)->mesh = NULL;
    (*self)->transform = Transform_identity();
    return true;
}

void Object3D_destroy(Object3D** self) {
    if (!self || !*self)
        return;
    Object3D* obj = *self;
    free(obj);
    *self = NULL;
}

void Object3D_set_position(Object3D* self, Vector3 position) {
    if (!self) {
        fprintf(stderr, "Object3D_set_mesh: Object3D* is NULL\n");
        return;
    }
    self->transform.position = position;
}

void Object3D_set_mesh(Object3D* self, Mesh* mesh) {
    if (!self) {
        fprintf(stderr, "Object3D_set_mesh: Object3D* is NULL\n");
        return;
    }
    self->mesh = mesh;
}

Mesh* Object3D_get_mesh(const Object3D* self) {
    if (!self) {
        fprintf(stderr, "Object3D_get_mesh: Object3D* is NULL\n");
        return NULL;
    }
    return self->mesh;
}

Transform Object3D_get_transform(const Object3D* self) {
    if (!self) {
        fprintf(stderr, "Object3D_get_transform: Object3D* is NULL\n");
        return (Transform){0};
    }
    return self->transform;
}

void Object3D_translate(Object3D* self, Vector3 delta) {
    Transform_translate(&self->transform, delta);
}

void Object3D_scale(Object3D* self, Vector3 delta) {
    Transform_scale(&self->transform, delta);
}
