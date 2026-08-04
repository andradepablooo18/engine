#include "scene/Object3D.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
struct Object3D {
        Mesh* mesh;
        Transform transform;
        Material* material;
};

bool Object3D_create(Object3D** self) {
    assert(self);
    *self = calloc(1, sizeof(Object3D));
    if (!*self) {
        fprintf(stderr, "Object3D_create: allocation failed\n");
        return false;
    }
    (*self)->mesh = NULL;
    (*self)->transform = Transform_identity();
    (*self)->material = NULL;
    return true;
}

void Object3D_destroy(Object3D** self) {
    assert(self);
    if (!*self)
        return;
    Object3D* obj = *self;
    free(obj);
    *self = NULL;
}

void Object3D_set_position(Object3D* self, Vector3 position) {
    assert(self);
    self->transform.position = position;
}

void Object3D_set_mesh(Object3D* self, Mesh* mesh) {
    assert(self);
    self->mesh = mesh;
}

void Object3D_set_material(Object3D* self, Material* material) {
    assert(self);
    self->material = material;
}

const Mesh* Object3D_get_mesh(const Object3D* self) {
    assert(self);
    return self->mesh;
}

const Transform* Object3D_get_transform(const Object3D* self) {
    assert(self);
    return &(self->transform);
}

const Material* Object3D_get_material(const Object3D* self) {
    assert(self);
    return self->material;
}

void Object3D_translate(Object3D* self, Vector3 delta) {
    assert(self);
    Transform_translate(&self->transform, delta);
}

void Object3D_scale(Object3D* self, Vector3 delta) {
    assert(self);
    Transform_scale(&self->transform, delta);
}

void Object3D_rotate(Object3D* self, Vector3 delta) {
    assert(self);
    Transform_rotate(&self->transform, delta);
}
