#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>

typedef struct Window {
        SDL_Window* handle;
        int width;
        int height;
        char* title;
} Window;

bool Window_create(Window** window, const char* title, int width, int height);
void Window_destroy(Window** window);

#endif
