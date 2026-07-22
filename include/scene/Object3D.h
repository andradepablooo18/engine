#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Transform.h"
#include "graphics/Mesh.h"
#include <stdbool.h>

typedef struct Object3D Object3D;

bool Object3D_create(Object3D** self);
void Object3D_destroy(Object3D** self);

void Object3D_set_position(Object3D* self, Vector3 position);
void Object3D_set_mesh(Object3D* self, Mesh* mesh);

Mesh* Object3D_get_mesh(const Object3D* self);
Transform Object3D_get_transform(const Object3D* self);

void Object3D_translate(Object3D* self, Vector3 delta);
void Object3D_scale(Object3D* self, Vector3 delta);
void Object3D_rotate(Object3D* self, Vector3 delta);

#endif
