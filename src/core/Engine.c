#include "core/Engine.h"
#include "core/Engine_config.h"
#include "core/colors.h"
#include "internal/Input_internal.h"
#include "internal/renderer/Renderer.h"
#include "internal/window/Window.h"
#include <stdlib.h>

struct Engine {
        Window* window;
        Renderer* renderer;
        Input* input;
        SDL_Event event;
        bool is_running;
};

static void Engine_process_input(Engine* self);
// static void Engine_update(Engine* e);
// static void Engine_draw(Engine* e);

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

    if (!Input_create(&e->input)) {
        Engine_destroy(engine);
        return false;
    }

    e->is_running = true;

    return true;
}

void Engine_destroy(Engine** engine) {
    if (!engine || !*engine)
        return;

    Engine* const e = *engine;
    if (e->input) {
        Input_destroy(&e->input);
    }
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

void Engine_run(Engine* e, const EngineCallbacks* callbacks) {
    if (!e) {
        fprintf(stderr, "Engine_run: Engine* is NULL\n");
        return;
    }
    if (!callbacks) {
        fprintf(stderr, "Engine_run: EngineCallbacks* is NULL\n");
        return;
    }

    while (e->is_running) {
        Engine_process_input(e);

        if (callbacks->update)
            callbacks->update(e);

        Renderer_clear(e->renderer, COLOR_BLACK);
        if (callbacks->draw)
            callbacks->draw(e);
        Renderer_present(e->renderer);
    }
}

static void Engine_process_input(Engine* self) {
    Input_begin_frame(self->input);

    while (SDL_PollEvent(&self->event)) {

        Input_process_event(self->input, &self->event);

        switch (self->event.type) {
            case SDL_EVENT_QUIT:
                self->is_running = false;
                return;
            default:
                break;
        }
    }
}

// static void Engine_update(Engine* e) {}

// static void Engine_draw(Engine* e) {}
//
Input* Engine_get_input(Engine* e) {
    if (!e) {
        fprintf(stderr,
                "Engine_get_input: invalid argument (Engine* e is NULL)\n");
        return NULL;
    }
    return e->input;
}

void Engine_draw_pixel(Engine* e, int x, int y, u32 color) {
    Renderer_draw_pixel(e->renderer, x, y, color);
}

void Engine_draw_line(Engine* e, Vector2 start, Vector2 end, u32 color) {
    Renderer_draw_line(e->renderer, start, end, color);
}

void Engine_draw_triangle(Engine* e, Triangle triangle, u32 color) {
    Renderer_draw_triangle(e->renderer, triangle, color);
}

void Engine_draw_triangle_wireframe(Engine* e, Triangle triangle, u32 color) {
    Renderer_draw_triangle_wireframe(e->renderer, triangle, color);
}

void Engine_draw_object3D(const Engine* e, const Camera* camera,
                          const Object3D* obj) {
    Renderer_draw_object3D(e->renderer, camera, obj);
}
