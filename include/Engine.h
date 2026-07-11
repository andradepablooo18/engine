#ifndef ENGINE_H
#define ENGINE_H

#include "Engine_config.h"
#include <stdbool.h>

typedef struct Engine Engine;

bool Engine_create(Engine** engine);
void Engine_run(Engine* e);
void Engine_destroy(Engine** engine);

#endif
