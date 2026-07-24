#ifndef RASTER_MODE_H
#define RASTER_MODE_H

#include <stdbool.h>

typedef enum RasterMode {
    RASTER_MODE_POINTS,
    RASTER_MODE_WIREFRAME,
    RASTER_MODE_SOLID,
} RasterMode;

static inline bool is_raster_mode(RasterMode raster_mode) {
    return raster_mode >= RASTER_MODE_POINTS &&
           raster_mode <= RASTER_MODE_SOLID;
}

#endif
