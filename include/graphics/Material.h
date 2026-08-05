#ifndef MATERIAL_H
#define MATERIAL_H

#include "graphics/Texture.h"

typedef struct Material Material;

bool Material_create(Material** self);
void Material_destroy(Material** self);

void Material_set_texture(Material* self, Texture* texture);
void Material_set_color(Material* self, Color color);

const Material* Material_get_default();
const Texture* Material_get_texture(const Material* self);
Color Material_get_color(const Material* self);

#endif
