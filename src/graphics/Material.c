#include "graphics/Material.h"
#include "core/Color.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Material {
        Texture* texture;
        Color color;
};

bool Material_create(Material** self) {
    assert(self);

    *self = calloc(1, sizeof(Material));
    if (!*self) {
        fprintf(stderr, "Material_create: Material allocation failed\n");
        return false;
    }

    (*self)->texture = NULL;
    (*self)->color = COLOR_WHITE;

    return true;
}

void Material_destroy(Material** self) {
    assert(self);
    if (!*self)
        return;

    free(*self);
    *self = NULL;
}

void Material_set_texture(Material* self, Texture* texture) {
    assert(self);

    self->texture = texture;
}

void Material_set_color(Material* self, Color color) {
    assert(self);

    self->color = color;
}

const Texture* Material_get_texture(const Material* self) {
    assert(self);

    return self->texture;
}

Color Material_get_color(const Material* self) {
    assert(self);

    return self->color;
}
