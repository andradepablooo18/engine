#include "Camera.h"
#include "Engine.h"
#include "Engine_config.h"
#include "Input.h"
#include "Mesh.h"
#include "Object3D.h"
#include "Transform.h"
#include "colors.h"
#include "keys.h"
#include "math/Matrix4.h"
#include "math/Vector4.h"
#include "math/common.h"
#include "mousebuttons.h"
#include <stdio.h>
#include <stdlib.h>

void setup(void);
void update(Engine* e);
void draw(Engine* e);
void destroy(void);

int main(void) {
    Mesh* cube_mesh = NULL;
    if (!Mesh_create_cube(&cube_mesh)) {
        return EXIT_FAILURE;
    }

    Object3D* cube;
    if (!Object3D_create(&cube)) {
        return EXIT_FAILURE;
    }
    Object3D_set_mesh(cube, cube_mesh);

    Object3D_destroy(&cube);
    Mesh_destroy(&cube_mesh);

    Engine* e = NULL;

    if (!Engine_create(&e)) {
        return EXIT_FAILURE;
    }

    EngineCallbacks callbacks = {.update = update, .draw = draw};

    setup();

    Engine_run(e, &callbacks);

    Engine_destroy(&e);

    destroy();

    return EXIT_SUCCESS;
}

void setup(void) {
    Camera_create(&camera);
    Object3D_create_cube(&cube);
}

void update(Engine* e) {}

void draw(Engine* e) { Engine_draw_object3D(e, &camera, &cube); }

void destroy(void) { Object3D_destroy(&cube); }
