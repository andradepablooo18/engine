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
//     // BOTH INSIDE = ACCEPT LINE
//     Engine_draw_line(e, (Vector2){-100.0f, 0.0f}, (Vector2){300.0f, 300.0f},
//                      COLOR_GREEN);
//     Engine_draw_line(e, (Vector2){300.0f, 300.0f}, (Vector2){-100.0f, 0.0f},
//                      COLOR_GREEN);
//
//     // BOTH OUTSIDE = REJECT LINE
//
//     // LEFT
//     Engine_draw_line(e, (Vector2){-100.0f, 0.0f},
//                      (Vector2){-100.0f, HEIGHT - 1}, COLOR_GREEN);
//     Engine_draw_line(e, (Vector2){-100.0f, HEIGHT - 1},
//                      (Vector2){-100.0f, 0.0f}, COLOR_GREEN);
//
//     // RIGHT
//     Engine_draw_line(e, (Vector2){WIDTH, 0.0f}, (Vector2){WIDTH, HEIGHT - 1},
//                      COLOR_GREEN);
//     Engine_draw_line(e, (Vector2){WIDTH, HEIGHT - 1}, (Vector2){WIDTH, 0.0f},
//                      COLOR_GREEN);
//
//     // TOP
//     Engine_draw_line(e, (Vector2){0.0f, -1.0f}, (Vector2){WIDTH - 1, -1.0f},
//                      COLOR_GREEN);
//     Engine_draw_line(e, (Vector2){WIDTH - 1, -1.0f}, (Vector2){0.0f, -1.0f},
//                      COLOR_GREEN);
//
//     // BOTTOM
//     Engine_draw_line(e, (Vector2){0.0f, HEIGHT}, (Vector2){WIDTH - 1,
//     HEIGHT},
//                      COLOR_GREEN);
//     Engine_draw_line(e, (Vector2){WIDTH - 1, HEIGHT}, (Vector2){0.0f,
//     HEIGHT},
//                      COLOR_GREEN);
//
//     // ONE INSIDE BUT OTHER OUTSIDE = CLIP
//
//     Vector2 center = {.x = (WIDTH - 1) / 2.0f, .y = (HEIGHT - 1) / 2.0f};
//
//     Engine_draw_line(e, center, (Vector2){-100.0f, 0.0f}, COLOR_GREEN);
//     Engine_draw_line(e, (Vector2){-100.0f, 0.0f}, center, COLOR_GREEN);
//
//     Engine_draw_line(e, center, (Vector2){WIDTH, 0.0f}, COLOR_GREEN);
//     Engine_draw_line(e, (Vector2){WIDTH, 0.0f}, center, COLOR_GREEN);
//
//     Engine_draw_line(e, center, (Vector2){0.0f, -1.0f}, COLOR_GREEN);
//     Engine_draw_line(e, (Vector2){0.0f, -1.0f}, center, COLOR_GREEN);
//
//     Engine_draw_line(e, center, (Vector2){0.0f, HEIGHT}, COLOR_GREEN);
//     Engine_draw_line(e, (Vector2){0.0f, HEIGHT}, center, COLOR_GREEN);
//
//     // BOTH OUTSIDE LINE STILL CROSS THE VIEWPORT
//     Engine_draw_line(e, (Vector2){-100.0f, -100.0f},
//                      (Vector2){WIDTH + 100, HEIGHT + 100}, COLOR_GREEN);
// }
