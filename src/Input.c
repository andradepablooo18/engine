#include "Input.h"
#include "internal/Input_internal.h"
#include <stdlib.h>

struct Input {
        bool current_keys[SDL_SCANCODE_COUNT];
        bool previous_keys[SDL_SCANCODE_COUNT];

        int mouse_x;
        int mouse_y;

        // bool current_mouse_buttons[MOUSE_BUTTON_COUNT];
        // bool previous_mouse_buttons[MOUSE_BUTTON_COUNT];
};

bool Input_create(Input** self) {
    if (!self) {
        fprintf(stderr, "Input_create: invalid argument (Input** is NULL)\n");
        return false;
    }

    *self = calloc(1, sizeof(Input));
    if (!*self) {
        fprintf(stderr, "Input_create: allocation failed\n");
        return false;
    }

    return true;
}

void Input_destroy(Input** self) {
    if (!self || !*self)
        return;
    free(*self);
    *self = NULL;
}

void Input_begin_frame(Input* self) {
    if (!self) {
        fprintf(stderr,
                "Input_begin_frame: invalid argument (Input* is NULL)\n");
        return;
    }
    memcpy(self->previous_keys, self->current_keys,
           sizeof(self->previous_keys));
}

void Input_process_event(Input* self, const SDL_Event* event) {
    if (!self || !event) {
        fprintf(stderr, "Input_process_event: invalid argument (Input* or "
                        "SDL_Event* is NULL)\n");
        return;
    }
    switch (event->type) {
        case SDL_EVENT_KEY_DOWN:
            self->current_keys[event->key.scancode] = true;
            break;
        case SDL_EVENT_KEY_UP:
            self->current_keys[event->key.scancode] = false;
            break;
        default:
            break;
    }
}

bool Input_is_key_down(const Input* self, Key key) {
    if (!self) {
        fprintf(stderr,
                "Input_is_key_down: invalid argument (Input* is NULL)\n");
        return false;
    }
    return self->current_keys[key];
}

bool Input_is_key_pressed(const Input* self, Key key) {
    if (!self) {
        fprintf(stderr,
                "Input_is_key_down: invalid argument (Input* is NULL)\n");
        return false;
    }
    return self->current_keys[key] && !self->previous_keys[key];
}

bool Input_is_key_released(const Input* self, Key key) {
    if (!self) {
        fprintf(stderr,
                "Input_is_key_down: invalid argument (Input* is NULL)\n");
        return false;
    }
    return !self->current_keys[key] && self->previous_keys[key];
}
