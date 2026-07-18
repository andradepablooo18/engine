#include "Input.h"
#include "internal/Input_internal.h"
#include "keys.h"
#include "mousebuttons.h"
#include "types.h"
#include <stdlib.h>
struct Input {
        bool current_keys[KEY_COUNT];
        bool previous_keys[KEY_COUNT];

        int mouse_x;
        int mouse_y;
        bool current_mouse_buttons[MOUSE_BUTTON_COUNT];
        bool previous_mouse_buttons[MOUSE_BUTTON_COUNT];
};

static Key SDL_to_key(SDL_Scancode key);
static MouseButton SDL_to_mouse_button(u8 button);
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
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            MouseButton button = SDL_to_mouse_button(event->button.button);
            if (button != MOUSE_BUTTON_UNKNOWN)
                self->current_mouse_buttons[button] = true;
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            MouseButton button = SDL_to_mouse_button(event->button.button);
            if (button != MOUSE_BUTTON_UNKNOWN)
                self->current_mouse_buttons[button] = false;
            break;
        }
        case SDL_EVENT_KEY_DOWN: {
            Key key = SDL_to_key(event->key.scancode);
            if (key != KEY_UNKNOWN)
                self->current_keys[key] = true;
            break;
        }
        case SDL_EVENT_KEY_UP: {
            Key key = SDL_to_key(event->key.scancode);
            if (key != KEY_UNKNOWN)
                self->current_keys[key] = false;
            break;
        }
        default:
            break;
    }
}

/*
 ************************************************
 ************************************************
 *
 * STATIC HELPERS
 *
 ************************************************
 ************************************************
 */

static Key SDL_to_key(SDL_Scancode key) {
    switch (key) {
        case SDL_SCANCODE_A:
            return KEY_A;
        case SDL_SCANCODE_B:
            return KEY_B;
        case SDL_SCANCODE_C:
            return KEY_C;
        case SDL_SCANCODE_D:
            return KEY_D;
        case SDL_SCANCODE_E:
            return KEY_E;
        case SDL_SCANCODE_F:
            return KEY_F;
        case SDL_SCANCODE_G:
            return KEY_G;
        case SDL_SCANCODE_H:
            return KEY_H;
        case SDL_SCANCODE_I:
            return KEY_I;
        case SDL_SCANCODE_J:
            return KEY_J;
        case SDL_SCANCODE_K:
            return KEY_K;
        case SDL_SCANCODE_L:
            return KEY_L;
        case SDL_SCANCODE_M:
            return KEY_M;
        case SDL_SCANCODE_N:
            return KEY_N;
        case SDL_SCANCODE_O:
            return KEY_O;
        case SDL_SCANCODE_P:
            return KEY_P;
        case SDL_SCANCODE_Q:
            return KEY_Q;
        case SDL_SCANCODE_R:
            return KEY_R;
        case SDL_SCANCODE_S:
            return KEY_S;
        case SDL_SCANCODE_T:
            return KEY_T;
        case SDL_SCANCODE_U:
            return KEY_U;
        case SDL_SCANCODE_V:
            return KEY_V;
        case SDL_SCANCODE_W:
            return KEY_W;
        case SDL_SCANCODE_X:
            return KEY_X;
        case SDL_SCANCODE_Y:
            return KEY_Y;
        case SDL_SCANCODE_Z:
            return KEY_Z;

        case SDL_SCANCODE_ESCAPE:
            return KEY_ESCAPE;
        case SDL_SCANCODE_SPACE:
            return KEY_SPACE;
        case SDL_SCANCODE_RETURN:
            return KEY_ENTER;
        case SDL_SCANCODE_TAB:
            return KEY_TAB;
        case SDL_SCANCODE_BACKSPACE:
            return KEY_BACKSPACE;

        case SDL_SCANCODE_UP:
            return KEY_UP;
        case SDL_SCANCODE_DOWN:
            return KEY_DOWN;
        case SDL_SCANCODE_LEFT:
            return KEY_LEFT;
        case SDL_SCANCODE_RIGHT:
            return KEY_RIGHT;

        case SDL_SCANCODE_LSHIFT:
            return KEY_LSHIFT;
        case SDL_SCANCODE_RSHIFT:
            return KEY_RSHIFT;

        case SDL_SCANCODE_LCTRL:
            return KEY_LCTRL;
        case SDL_SCANCODE_RCTRL:
            return KEY_RCTRL;

        case SDL_SCANCODE_LALT:
            return KEY_LALT;
        case SDL_SCANCODE_RALT:
            return KEY_RALT;

        default:
            fprintf(
                stderr,
                "Alert: Unknow key has been detected\n"); // this is a warning
            return KEY_UNKNOWN;
    }
}

static MouseButton SDL_to_mouse_button(u8 button) {
    switch (button) {
        case SDL_BUTTON_LEFT:
            return MOUSE_BUTTON_LEFT;
        case SDL_BUTTON_MIDDLE:
            return MOUSE_BUTTON_MIDDLE;
        case SDL_BUTTON_RIGHT:
            return MOUSE_BUTTON_RIGHT;
        case SDL_BUTTON_X1:
            return MOUSE_BUTTON_X1;
        case SDL_BUTTON_X2:
            return MOUSE_BUTTON_X2;
        default:
            fprintf(
                stderr,
                "Alert: Unknow mouse button hast been detected\n"); // this is a
                                                                    // warning
            return MOUSE_BUTTON_UNKNOWN;
    }
}

static inline bool is_key(Key key) { return key > 0 && key < KEY_COUNT; }

static inline bool is_mouse_button(MouseButton button) {
    return button > 0 && button < MOUSE_BUTTON_COUNT;
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

bool Input_is_mouse_button_down(const Input* self, MouseButton button) {
    if (!self) {
        fprintf(stderr, "Input_is_mouse_button_down: invalid argument "
                        "(Input* is NULL)\n");
        return false;
    }
    if (!is_mouse_button(button)) {
        fprintf(stderr, "Input_is_mouse_button_down: invalid argument "
                        "(MouseButton is not valid)\n");
        return false;
    }
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

void Input_get_mouse_position(const Input* self, int* x, int* y) {
    if (!self || !x || !y) {
        fprintf(stderr, "Input_get_mouse_position: invalid argument (Input* or "
                        "int* is NULL)\n");
        return;
    }
    *x = self->mouse_x;
    *y = self->mouse_y;
}
