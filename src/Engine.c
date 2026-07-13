#include "Engine.h"
#include "Window.h"
#include <stdio.h>
#include <stdlib.h>

struct Engine {
        Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;
        bool is_running;
};

static void Engine_process_input(Engine* e);
static void Engine_update(void);
static void Engine_draw(void);

bool Engine_create(Engine** engine) {
    if (!engine) {
        fprintf(stderr, "Engine_create: invalid argument\n");
        return false;
    }

    *engine = calloc(1, sizeof(Engine));
    if (!*engine) {
        fprintf(stderr, "Engine_create: allocation failed\n");
        return false;
    }

    Engine* const e = *engine;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        Engine_destroy(engine);
        return false;
    }

    if (!Window_create(&e->window, ENGINE_TITLE, WIDTH, HEIGHT)) {
        Engine_destroy(engine);
        return false;
    }

    e->renderer = SDL_CreateRenderer(Window_get_handle(e->window), NULL);
    if (!e->renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        Engine_destroy(engine);
        return false;
    }

    e->is_running = true;

    return true;
}

void Engine_run(Engine* e) {
    while (e->is_running) {
        Engine_process_input(e);
        Engine_update();
        Engine_draw();
    }
}

static void Engine_process_input(Engine* e) {
    while (SDL_PollEvent(&e->event)) {
        switch (e->event.type) {
            case SDL_EVENT_QUIT:
                e->is_running = false;
                return;
            default:
                break;
        }
    }
}

static void Engine_update(void) {}

static void Engine_draw(void) {}

void Engine_destroy(Engine** engine) {
    if (!engine || !*engine)
        return;

    Engine* const e = *engine;
    if (e->renderer) {
        SDL_DestroyRenderer(e->renderer);
        e->renderer = NULL; // this could be deleted
    }
    if (e->window) {
        Window_destroy(&e->window);
    }
    SDL_Quit();
    free(e);
    *engine = NULL;
}
