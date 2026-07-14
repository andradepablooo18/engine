#ifndef INPUT_H
#define INPUT_H

#include "types.h"
#include <SDL3/SDL.H>
#include <stdbool.h>

typedef struct Input Input;

bool Input_create(Input** self);
void Input_destroy(Input** self);
void Input_begin_frame(Input* self);
void Input_process_event(Input* self, const SDL_Event* event);
bool Input_is_key_down(const Input* self, SDL_Scancode key);
bool Input_is_key_pressed(const Input* self, SDL_Scancode key);
bool Input_is_key_released(const Input* self, SDL_Scancode key);
// bool Input_is_mouse_button_down(Input* self, MouseButton button);
// void Input_get_mouse_position(Input* self, i32* x, i32* y);

#endif
