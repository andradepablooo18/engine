#include "Engine.h"
#include "Engine_config.h"
#include "Renderer.h"
#include "Window.h"
#include "colors.h"
#include <stdio.h>
#include <stdlib.h>

struct Engine {
        Window* window;
        Renderer* renderer;
        SDL_Event event;
        bool is_running;
};

static void Engine_process_input(Engine* e);
static void Engine_update();
static void Engine_draw(Engine* e);

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

    if (!Renderer_create(&e->renderer, e->window)) {
        Engine_destroy(engine);
        return false;
    }

    e->is_running = true;

    return true;
}

void Engine_run(Engine* e) {
    if (!e) {
        fprintf(stderr, "Engine_run: argument is NULL\n");
        return;
    }

    while (e->is_running) {
        Engine_process_input(e);
        Engine_update();
        Engine_draw(e);
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

static void Engine_update() {}

static void Engine_draw(Engine* e) {
    Renderer_clear(e->renderer, COLOR_BLACK);
    Renderer_draw_pixel(e->renderer, 10, 10, COLOR_WHITE);
    Renderer_present(e->renderer);
}

void Engine_destroy(Engine** engine) {
    if (!engine || !*engine)
        return;

    Engine* const e = *engine;
    if (e->renderer) {
        Renderer_destroy(&e->renderer);
    }
    if (e->window) {
        Window_destroy(&e->window);
    }
    SDL_Quit();
    free(e);
    *engine = NULL;
}
