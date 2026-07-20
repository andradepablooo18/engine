#include "Camera.h"
#include "Engine.h"
#include "Input.h"
#include "Mesh.h"
#include "Object3D.h"
#include "Transform.h"
#include "colors.h"
#include "keys.h"
#include "math/Matrix4.h"
#include "mousebuttons.h"
#include <stdio.h>
#include <stdlib.h>

void setup(void);
void update(Engine* e);
void draw(Engine* e);

int main(void) {
    Engine* e = NULL;

    if (!Engine_create(&e)) {
        return EXIT_FAILURE;
    }

    EngineCallbacks callbacks = {.update = update, .draw = draw};

    setup();

    Engine_run(e, &callbacks);

    Engine_destroy(&e);

    return EXIT_SUCCESS;
}

void setup(void) {
    Camera camera;
    Camera_create(&camera);

    Object3D cube;
    Object3D_create_cube(&cube);

    // Bring cube to world space
    Matrix4 model_matrix = Transform_get_matrix(&cube.transform);

    // Bring cube to view space
    Matrix4 view_matrix = Camera_get_view_matrix(&camera);

    // Bring cube to clip space
    // Matrix4 projection_matrix = Matrix4_projection();

    // Bring cube to Normalized Device Coordinates (NDC)
    // Perspective divide

    // Viewport transform

    // Cube in screen space

    Object3D_destroy(&cube);
}

void update(Engine* e) {}

void draw(Engine* e) {}
