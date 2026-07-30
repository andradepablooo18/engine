// // ### ORIGINAL MAIN TEMPLATE ###
// #include "core/Engine.h"
// #include "core/types.h"
// #include <stdio.h>
// #include <stdlib.h>
//
// void update(Engine* e, f32 dt);
// void draw(Engine* e);
//
// int main(void) {
//     Engine* e = NULL;
//     if (!Engine_create(&e)) {
//         return EXIT_FAILURE;
//     }
//     EngineCallbacks callbacks = {.update = update, .draw = draw};
//     Engine_run(e, &callbacks);
//     Engine_destroy(&e);
//     return EXIT_SUCCESS;
// }
//
// void update(Engine* e, f32 dt) {}
//
// void draw(Engine* e) {}

#include "core/Engine.h"
#include "core/types.h"
#include "graphics/Camera.h"
#include "graphics/Mesh.h"
#include "graphics/RasterMode.h"
#include "input/Input.h"
#include "input/keys.h"
#include "math/common.h"
#include "scene/Object3D.h"
#include <stdio.h>
#include <stdlib.h>

#define SPEED 10.0f
#define ZOOM_SPEED 0.1f
#define SENSITIVITY 0.001f
#define ROTATION_SPEED HALF_PI
#define INV_SQRT2 0.70710678

bool setup(void);
void update(Engine* e, f32 dt);
void draw(Engine* e);
void destroy(void);

void move_cube(Input* input, f32 dt, f32 velocity);
void zoom_cube_with_mouse_wheel(Input* input, f32 zoom_speed);
void rotate_cube_when_pressed(Input* input, f32 sensitivity);
void rotate_cube_animation(f32 dt, f32 rotation_speed);

Input* input = NULL;
Camera* camera = NULL;
Mesh* cube_mesh = NULL;
Object3D* cube = NULL;
RasterMode raster_mode = -1;

int main(void) {
    Engine* e = NULL;
    if (!Engine_create(&e)) {
        return EXIT_FAILURE;
    }
    Engine_set_raster_mode(e, RASTER_MODE_SOLID);
    if (!setup()) {
        return EXIT_FAILURE;
    }
    input = Engine_get_input(e);
    raster_mode = Engine_get_raster_mode(e);

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
    Object3D_set_position(cube, (Vector3){0.0f, 0.0f, -10.0f});
    return true;
}

void update(Engine* e, f32 dt) {
    // Switch between raster_mode
    if (Input_is_key_pressed(input, KEY_R)) {
        raster_mode = (raster_mode + 1) % 3;
        Engine_set_raster_mode(e, raster_mode);
    }

    move_cube(input, dt, SPEED);
    zoom_cube_with_mouse_wheel(input, ZOOM_SPEED);
    rotate_cube_when_pressed(input, SENSITIVITY);
    // rotate_cube_animation(dt, ROTATION_SPEED);
}

void draw(Engine* e) { Engine_draw_object3D(e, camera, cube); }

void destroy(void) {
    Camera_destroy(&camera);
    Mesh_destroy(&cube_mesh);
    Object3D_destroy(&cube);
}

void move_cube(Input* input, f32 dt, f32 speed) {
    f32 dx = 0.0f;
    f32 dy = 0.0f;
    if (Input_is_key_down(input, KEY_W)) {
        dy = 1;
    }
    if (Input_is_key_down(input, KEY_S)) {
        dy = -1;
    }
    if (Input_is_key_down(input, KEY_A)) {
        dx = -1;
    }
    if (Input_is_key_down(input, KEY_D)) {
        dx = 1;
    }
    if (dx != 0.0f && dy != 0.0f) {
        dx *= INV_SQRT2;
        dy *= INV_SQRT2;
    }
    Object3D_translate(cube, (Vector3){dt * dx * speed, dt * dy * speed, 0.0f});
}

void zoom_cube_with_mouse_wheel(Input* input, f32 zoom_speed) {
    int wheel_y = Input_get_wheel_y(input);
    Camera_translate(camera, (Vector3){0, 0, -wheel_y * zoom_speed});
}

void rotate_cube_when_pressed(Input* input, f32 sensivity) {
    if (Input_is_mouse_button_down(input, MOUSE_BUTTON_LEFT)) {
        Vector2 delta = Input_get_mouse_delta_position(input);
        Object3D_rotate(
            cube, (Vector3){delta.y * sensivity, delta.x * sensivity, 0.0f});
    }
}

void rotate_cube_animation(f32 dt, f32 rotation_speed) {
    Object3D_rotate(cube,
                    (Vector3){dt * rotation_speed, dt * rotation_speed, 0.0f});
}
