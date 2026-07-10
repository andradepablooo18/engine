#ifndef ENGINE_H
#define ENGINE_H

#include "Engine_config.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

typedef struct Engine {
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Event event;
        bool is_running;
} Engine;

bool Engine_create(Engine** engine);
void Engine_run(Engine* e);
void Engine_destroy(Engine** engine);

#endif
