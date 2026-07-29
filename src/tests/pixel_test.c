// #include "core/Color.h"
// #include "core/Engine.h"
// #include "core/Engine_config.h"
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
// void draw(Engine* e) {
//     // LEFT
//     Engine_draw_pixel(e, -100, 0, COLOR_MAGENTA);
//     // RIGHT
//     Engine_draw_pixel(e, WIDTH, 0, COLOR_MAGENTA);
//     // TOP
//     Engine_draw_pixel(e, 0, -100, COLOR_MAGENTA);
//     // BOTTOM
//     Engine_draw_pixel(e, 0, HEIGHT, COLOR_MAGENTA);
//
//     // LEFT & TOP
//     Engine_draw_pixel(e, -100, -100, COLOR_MAGENTA);
//     // RIGHT & TOP
//     Engine_draw_pixel(e, WIDTH, -100, COLOR_MAGENTA);
//
//     // LEFT & BOTTOM
//     Engine_draw_pixel(e, -100, HEIGHT, COLOR_MAGENTA);
//     // RIGHT & TOP
//     Engine_draw_pixel(e, WIDTH, HEIGHT, COLOR_MAGENTA);
// }
