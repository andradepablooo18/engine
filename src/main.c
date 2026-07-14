#include "Engine.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

void update(Engine* e);
void draw(Engine* e);

int main(void) {
    Engine* e = NULL;

    if (!Engine_create(&e)) {
        return EXIT_FAILURE;
    }

    EngineCallbacks callbacks = {.update = update, .draw = draw};

    Engine_run(e, &callbacks);

    Engine_destroy(&e);

    return EXIT_SUCCESS;
}

void update(Engine* e) {}

void draw(Engine* e) { Engine_draw_pixel(e, 10, 10, COLOR_WHITE); };
