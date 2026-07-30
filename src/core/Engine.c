#include "core/Engine.h"
#include "core/Color.h"
#include "core/Engine_config.h"
#include "graphics/RasterMode.h"
#include "internal/Input_internal.h"
#include "internal/renderer/Renderer.h"
#include "internal/window/Window.h"
#include <assert.h>
#include <stdlib.h>

struct Engine {
        Window* window;
        Renderer* renderer;
        Input* input;
        SDL_Event event;

        Uint64 previous_time;
        Uint64 current_time;

        bool is_running;
};

static void Engine_process_input(Engine* self);
static f32 get_delta_time(Uint64* previous_time);

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

    // Rasterization mode is wireframe by default
    if (!Renderer_create(&e->renderer, e->window, RASTER_MODE_WIREFRAME)) {
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

    Uint64 previous_time = SDL_GetTicksNS();

    while (e->is_running) {
        f32 dt = get_delta_time(&previous_time);

        Engine_process_input(e);

        if (callbacks->update)
            callbacks->update(e, dt);

        Renderer_clear(e->renderer, COLOR_BLACK);
        if (callbacks->draw)
            callbacks->draw(e);
        Renderer_present(e->renderer);
    }
}

Input* Engine_get_input(Engine* e) {
    if (!e) {
        fprintf(stderr,
                "Engine_get_input: invalid argument (Engine* e is NULL)\n");
        return NULL;
    }
    return e->input;
}

void Engine_set_raster_mode(Engine* self, RasterMode raster_mode) {
    if (!self) {
        fprintf(stderr,
                "Engine_set_raster_mode: invalid argument (Engine* is NULL)");
        return;
    }
    if (!is_raster_mode(raster_mode)) {
        fprintf(stderr, "Engine_set_raster_mode: unknown RasterMode\n");
        // alert the user that raster mode keeps unchanged?
        return;
    }
    Renderer_set_raster_mode(self->renderer, raster_mode);
}

RasterMode Engine_get_raster_mode(const Engine* self) {
    if (!self) {
        fprintf(stderr,
                "Engine_get_raster_mode: invalid argument (Engine* is NULL)\n");
        return -1;
    }
    return Renderer_get_raster_mode(self->renderer);
}

void Engine_draw_pixel(const Engine* self, i32 x, i32 y, Color color) {
    assert(self);
    Renderer_draw_pixel(self->renderer, x, y, color);
}

void Engine_draw_line(const Engine* self, Vector2 a, Vector2 b, Color color) {
    assert(self);
    Renderer_draw_line(self->renderer, a, b, color);
}

void Engine_draw_triangle(const Engine* self, Triangle triangle) {
    assert(self);
    Renderer_draw_triangle(self->renderer, triangle);
}

void Engine_draw_object3D(const Engine* self, const Camera* camera,
                          const Object3D* obj) {
    assert(self);
    assert(camera);
    assert(obj);
    Renderer_draw_object3D(self->renderer, camera, obj);
}

/*
 ************************************************
 ************************************************
 *
 * STATIC HELPERS
 *
 ************************************************
 ************************************************
 */

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

static f32 get_delta_time(Uint64* previous_time) {
    Uint64 current_time = SDL_GetTicksNS();
    f32 dt = (current_time - *previous_time) / 1000000000.0f;
    *previous_time = current_time;
    return dt;
}
