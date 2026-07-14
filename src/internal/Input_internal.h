#ifndef INPUT_INTERNAL_H
#define INPUT_INTERNAL_H

#include "Input.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

bool Input_create(Input** self);
void Input_destroy(Input** self);
void Input_begin_frame(Input* self);
void Input_process_event(Input* self, const SDL_Event* event);

#endif
