#include "Engine.h"
#include "Input.h"
#include "colors.h"
#include "keys.h"
#include "mousebuttons.h"
#include <stdio.h>
#include <stdlib.h>

void update(Engine* e);
void draw(Engine* e);

int x = 0;
int y = 0;

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

void update(Engine* e) {
    Input* input = Engine_get_input(e);
    if (Input_is_mouse_button_pressed(input, MOUSE_BUTTON_LEFT)) {
        printf("Left click is pressed!\n");
    }
    if (Input_is_mouse_button_released(input, MOUSE_BUTTON_LEFT)) {
        printf("Left click is released!\n");
    }
}

void draw(Engine* e) {
    // SOME CODE =)
    Vector2 start = Vector2_create(100.0f, 100.0f);
    Vector2 end = Vector2_create(200.0f, 0.0f);
    Engine_draw_line(e, start, end, COLOR_RED);
}
