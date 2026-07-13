#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>

typedef struct Window Window;

bool Window_create(Window** window, const char* title, int width, int height);
void Window_destroy(Window** window);
SDL_Window* Window_get_handle(Window* self);

#endif
