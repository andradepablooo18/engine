#include "Input.h"
#include "internal/Input_internal.h"
#include <stdlib.h>

struct Input {
        bool current_keys[KEY_COUNT];
        bool previous_keys[KEY_COUNT];

        int mouse_x;
        int mouse_y;
        bool current_mouse_buttons[MOUSE_BUTTON_COUNT];
        bool previous_mouse_buttons[MOUSE_BUTTON_COUNT];
};

// where i put this shits?
static bool is_key(Key key);
static bool is_mouse_button(MouseButton button);

/*
 ************************************************
 ************************************************
 *
 * PRIVATE API
 *
 ************************************************
 ************************************************
 */

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
    memcpy(self->previous_mouse_buttons, self->current_mouse_buttons,
           sizeof(self->previous_mouse_buttons));
}

void Input_process_event(Input* self, const SDL_Event* event) {
    if (!self || !event) {
        fprintf(stderr, "Input_process_event: invalid argument (Input* or "
                        "SDL_Event* is NULL)\n");
        return;
    }
    switch (event->type) {
        case SDL_EVENT_MOUSE_MOTION:
            self->mouse_x = event->motion.x;
            self->mouse_y = event->motion.y;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            self->current_mouse_buttons[event->button.button] = true;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            self->current_mouse_buttons[event->button.button] = false;
            break;
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

/*
 ************************************************
 ************************************************
 *
 * PUBLIC API
 *
 ************************************************
 ************************************************
 */

/*
 ************************************************
 *
 * KEYBOARD EVENTS
 *
 ************************************************
 */

/* Just a helper for public API keyboard events functions */
static bool is_key(Key key) {
    return (key >= 0 && key < KEY_COUNT) ? true : false;
}

bool Input_is_key_down(const Input* self, Key key) {
    if (!self) {
        fprintf(stderr,
                "Input_is_key_down: invalid argument (Input* is NULL)\n");
        return false;
    }
    if (!is_key(key)) {
        fprintf(stderr,
                "Input_is_key_down: invalid argument (key is not valid)\n");
        return false;
    }
    return self->current_keys[key];
}

bool Input_is_key_pressed(const Input* self, Key key) {
    if (!self) {
        fprintf(stderr,
                "Input_is_key_pressed: invalid argument (Input* is NULL)\n");
        return false;
    }
    if (!is_key(key)) {
        fprintf(stderr,
                "Input_is_key_pressed: invalid argument (key is not valid)\n");
        return false;
    }
    return self->current_keys[key] && !self->previous_keys[key];
}

bool Input_is_key_released(const Input* self, Key key) {
    if (!self) {
        fprintf(stderr,
                "Input_is_key_released: invalid argument (Input* is NULL)\n");
        return false;
    }
    if (!is_key(key)) {
        fprintf(stderr,
                "Input_is_key_released: invalid argument (key is not valid)\n");
        return false;
    }
    return !self->current_keys[key] && self->previous_keys[key];
}

/*
 ************************************************
 *
 * MOUSE EVENTS
 *
 ************************************************
 */

/* Just a helper for public API mouse events functions */
static bool is_mouse_button(MouseButton button) {
    return (button >= 0 && button < MOUSE_BUTTON_COUNT) ? true : false;
}

bool Input_is_mouse_button_down(Input* self, MouseButton button) {
    if (!self) {
        fprintf(
            stderr,
            "Input_is_mouse_button_down: invalid argument (Input* is NULL)\n");
        return false;
    }
    if (!is_mouse_button(button)) {
        fprintf(stderr, "Input_is_mouse_button_down: invalid argument "
                        "(MouseButton is not valid)\n");
        return false;
    }
    printf("Input_is_mouse_button_down: %d\n", button);
    printf("%d\n", self->current_mouse_buttons[button]);
    return self->current_mouse_buttons[button];
}

bool Input_is_mouse_button_pressed(const Input* self, MouseButton button) {
    if (!self) {
        fprintf(stderr, "Input_is_mouse_button_pressed: invalid argument "
                        "(Input* is NULL)\n");
        return false;
    }
    if (!is_mouse_button(button)) {
        fprintf(stderr, "Input_is_mouse_button_pressed: invalid argument "
                        "(MouseButton is not valid)\n");
        return false;
    }
    return self->current_mouse_buttons[button] &&
           !self->previous_mouse_buttons[button];
}

bool Input_is_mouse_button_released(const Input* self, MouseButton button) {
    if (!self) {
        fprintf(stderr, "Input_is_mouse_button_released: invalid argument "
                        "(Input* is NULL)\n");
        return false;
    }
    if (!is_mouse_button(button)) {
        fprintf(stderr, "Input_is_mouse_button_released: invalid argument "
                        "(MouseButton is not valid)\n");
        return false;
    }
    return !self->current_mouse_buttons[button] &&
           self->previous_mouse_buttons[button];
}

void Input_get_mouse_position(Input* self, int* x, int* y) {
    if (!self || !x || !y) {
        fprintf(stderr, "Input_get_mouse_position: invalid argument (Input* or "
                        "int* is NULL)\n");
        return;
    }
    *x = self->mouse_x;
    *y = self->mouse_y;
}
