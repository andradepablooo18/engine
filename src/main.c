#include "core/Engine.h"
#include "graphics/Camera.h"
#include "graphics/Mesh.h"
#include "scene/Object3D.h"
#include <stdio.h>
#include <stdlib.h>

bool setup(void);
void update(Engine* e);
void draw(Engine* e);
void destroy(void);

Camera* camera = NULL;
Mesh* cube_mesh = NULL;
Object3D* cube = NULL;

int main(void) {
    Engine* e = NULL;
    if (!Engine_create(&e)) {
        return EXIT_FAILURE;
    }

    if (!setup()) {
        return EXIT_FAILURE;
    }

    EngineCallbacks callbacks = {.update = update, .draw = draw};
    Engine_run(e, &callbacks);

    Engine_destroy(&e);

    destroy();

    return EXIT_SUCCESS;
}

bool setup(void) {
    if (!Camera_create(&camera)) {
        return false;
    }
    if (!Mesh_create_cube(&cube_mesh)) {
        return false;
    }
    if (!Object3D_create(&cube)) {
        return false;
    }
    Object3D_set_mesh(cube, cube_mesh);
    return true;
}

void update(Engine* e) {}

void draw(Engine* e) { Engine_draw_object3D(e, camera, cube); }

void destroy(void) {
    Camera_destroy(&camera);
    Mesh_destroy(&cube_mesh);
    Object3D_destroy(&cube);
}
