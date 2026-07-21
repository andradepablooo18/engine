#ifndef WINDOW_H
#define WINDOW_H

#include "core/types.h"
#include <SDL3/SDL.h>

typedef struct Window Window;

bool Window_create(Window** window, const char* title, i32 width, i32 height);
void Window_destroy(Window** window);
SDL_Window* Window_get_handle(const Window* self);
i32 Window_get_width(const Window* self);
i32 Window_get_height(const Window* self);

#endif
