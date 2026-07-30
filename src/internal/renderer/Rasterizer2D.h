#ifndef RASTERIZER2D_H
#define RASTERIZER2D_H

#include "core/Color.h"
#include "core/types.h"
#include "math/Vector2.h"

void Rasterizer2D_draw_pixel(i32 x, i32 y, Color color,
                             Color* const frame_buffer, i32 width, i32 height);
void Rasterizer2D_draw_line(Vector2 a, Vector2 b, Color color,
                            Color* const frame_buffer, i32 width, i32 height);
void Rasterizer2D_draw_triangle_solid(Vector2 a, Vector2 b, Vector2 c,
                                      Color* const frame_buffer, i32 width,
                                      i32 height);

#endif
