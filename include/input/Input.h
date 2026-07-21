#ifndef INPUT_H
#define INPUT_H

#include "keys.h"
#include "mousebuttons.h"
#include <stdbool.h>

typedef struct Input Input;

/*
 ************************************************
 *
 * KEYBOARD EVENTS
 *
 ************************************************
 */

bool Input_is_key_down(const Input* self, Key key);
bool Input_is_key_pressed(const Input* self, Key key);
bool Input_is_key_released(const Input* self, Key key);

/*
 ************************************************
 *
 * MOUSE EVENTS
 *
 ************************************************
 */

bool Input_is_mouse_button_down(const Input* self, MouseButton button);
bool Input_is_mouse_button_pressed(const Input* self, MouseButton button);
bool Input_is_mouse_button_released(const Input* self, MouseButton button);
void Input_get_mouse_position(const Input* self, int* x, int* y);

#endif
