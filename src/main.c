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
#include "input/Input.h"
#include <stdio.h>
#include <stdlib.h>

bool setup(void);
void update(Engine* e, f32 dt);
void draw(Engine* e);
void destroy(void);

int main(void) {
    Engine* e = NULL;
    if (!Engine_create(&e)) {
        return EXIT_FAILURE;
    }
    Engine_set_raster_mode(e, RASTER_MODE_SOLID);

    if (!setup()) {
        return EXIT_FAILURE;
    }

    EngineCallbacks callbacks = {.update = update, .draw = draw};
    Engine_run(e, &callbacks);

    Engine_destroy(&e);

    destroy();

    return EXIT_SUCCESS;
}

bool setup(void) { return true; }

void update(Engine* e, f32 dt) {
    Input* input = Engine_get_input(e);
    // Switch between raster_mode
    if (Input_is_key_pressed(input, KEY_R)) {
        RasterMode current_raster_mode = Engine_get_raster_mode(e);
        current_raster_mode = (current_raster_mode + 1) % 3;
        Engine_set_raster_mode(e, current_raster_mode);
    }
}

void draw(Engine* e) {
    Triangle t1 = {
        .p0 = {-100.0f, 0.0f}, .p1 = {200.0f, 0.0f}, .p2 = {200.0f, 400.0f}};
    Engine_draw_triangle(e, t1);
}

void destroy(void) {}
