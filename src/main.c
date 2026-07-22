#include "core/Engine.h"
#include "core/types.h"
#include "input/Input.h"
#include <stdio.h>
#include <stdlib.h>

bool setup(void);
void update(Input* input, f32 dt);
void draw(Engine* e);
void destroy(void);

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

bool setup(void) { return true; }

void update(Input* input, f32 dt) {}

void draw(Engine* e) {}

void destroy(void) {}
