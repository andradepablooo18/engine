#include "Window.h"
#include <_string.h>
#include <stdlib.h>
#include <string.h>

struct Window {
        SDL_Window* handle;
        int width;
        int height;
        char* title;
};

bool Window_create(Window** window, const char* title, int width, int height) {
    if (!window) {
        fprintf(stderr, "Window_create: invalid argument\n");
        return false;
    }

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
    if (!window || !*window)
        return;

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

SDL_Window* Window_get_handle(Window* self) {
    if (!self) {
        fprintf(stderr, "Window_get_handle: invalid argument\n");
        return NULL;
    }
    return self->handle;
}
