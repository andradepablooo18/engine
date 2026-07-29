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
