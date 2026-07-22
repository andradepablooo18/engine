#ifndef INPUT_H
#define INPUT_H

#include "core/types.h"
#include "keys.h"
#include "math/Vector2.h"
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

Vector2 Input_get_mouse_position(const Input* self);
Vector2 Input_get_mouse_delta_position(const Input* self);

/*
 ************************************************
 *
 * MOUSE WHEEL EVENTS
 *
 ************************************************
 */

f32 Input_get_wheel_x(const Input* self);
f32 Input_get_wheel_y(const Input* self);

#endif
