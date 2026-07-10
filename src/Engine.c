#include "Engine.h"
#include <stdio.h>
#include <stdlib.h>

static void Engine_process_input(Engine* e);
static void Engine_update(void);
static void Engine_draw(void);

bool Engine_create(Engine** engine) {
    if (engine == NULL) {
        fprintf(stderr, "Engine_create failed");
        return false;
    }

    *engine = calloc(1, sizeof(Engine));
    if (!*engine) {
        fprintf(stderr, "Engine_create failed\n");
        return false;
    }

    Engine* const e = *engine;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        Engine_destroy(engine);
        return false;
    }

    e->window = SDL_CreateWindow(ENGINE_TITLE, WIDTH, HEIGHT, 0);
    if (!e->window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        Engine_destroy(engine);
        return false;
    }

    e->renderer = SDL_CreateRenderer(e->window, NULL);
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
    if (engine == NULL || *engine == NULL)
        return;

    Engine* const e = *engine;
    if (e->renderer) {
        SDL_DestroyRenderer(e->renderer);
        e->renderer = NULL;
    }
    if (e->window) {
        SDL_DestroyWindow(e->window);
        e->window = NULL;
    }
    SDL_Quit();
    free(e);
    *engine = NULL;
}
