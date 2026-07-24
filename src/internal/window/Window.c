#include "Window.h"
#include <stdlib.h>
#include <string.h>

struct Window {
        SDL_Window* handle;
        i32 width;
        i32 height;
        char* title;
};

bool Window_create(Window** window, const char* title, i32 width, i32 height) {
    *window = calloc(1, sizeof(Window));
    if (!*window) {
        fprintf(stderr, "Window_create: allocation failed\n");
        return false;
    }

    Window* const w = *window;

    w->title = strdup(title);
    if (!w->title) {
        fprintf(stderr,
                "Window_create failed while assigning window's title\n");
        Window_destroy(window);
        return false;
    }
    w->width = width;
    w->height = height;

    w->handle = SDL_CreateWindow(w->title, w->width, w->height, 0);
    if (!w->handle) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        Window_destroy(window);
        return false;
    }

    return true;
}

void Window_destroy(Window** window) {
    Window* const w = *window;
    if (w->handle) {
        SDL_DestroyWindow(w->handle);
        w->handle = NULL; // this could be deleted
    }
    if (w->title) {
        free((void*)w->title);
        w->title = NULL; // this could be deleted
    }
    free(w);
    *window = NULL;
}

SDL_Window* Window_get_handle(const Window* self) { return self->handle; }

i32 Window_get_width(const Window* self) { return self->width; }

i32 Window_get_height(const Window* self) { return self->height; }
