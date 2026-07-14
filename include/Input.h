#ifndef INPUT_H
#define INPUT_H

#include "keys.h"
#include "types.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct Input Input;

bool Input_is_key_down(const Input* self, Key key);
bool Input_is_key_pressed(const Input* self, Key key);
bool Input_is_key_released(const Input* self, Key key);
// bool Input_is_mouse_button_down(Input* self, MouseButton button);
// void Input_get_mouse_position(Input* self, i32* x, i32* y);

#endif
