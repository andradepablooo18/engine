#include "Rasterizer2D.h"
#include "core/Color.h"
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* Define region codes for 9 region grid for Cohen Sutherland line clipping
 * algorithm */
#define INSIDE 0 /* 0000 */
#define LEFT 1   /* 0001 */
#define RIGHT 2  /* 0010 */
#define BOTTOM 4 /* 0100 */
#define TOP 8    /* 1000 */

static inline void put_pixel(i32 x, i32 y, Color color,
                             Color* const frame_buffer, i32 width);
static bool cohenSutherlandClip(Vector2* a, Vector2* b, f32 x_min, f32 y_min,
                                f32 x_max, f32 y_max);
static i32 compute_code(Vector2 v, f32 x_min, f32 y_min, f32 x_max, f32 y_max);
// static void DDA(Vector2 a, Vector2 b, Color color, Color* const frame_buffer,
//                 i32 width);
static void bresenham(Vector2 a, Vector2 b, Color color,
                      Color* const frame_buffer, i32 width);

void Rasterizer2D_draw_pixel(i32 x, i32 y, Color color,
                             Color* const frame_buffer, i32 width, i32 height) {
    assert(frame_buffer);
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    frame_buffer[y * width + x] = color;
}

static inline void put_pixel(i32 x, i32 y, Color color,
                             Color* const frame_buffer, i32 width) {
    frame_buffer[y * width + x] = color;
}

void Rasterizer2D_draw_line(Vector2 a, Vector2 b, Color color,
                            Color* const frame_buffer, i32 width, i32 height) {
    assert(frame_buffer);
    /* Check if line was accepted (even if it was clipped) in order to be drawn,
     * or rejected in case it is outside the buffer */
    if (cohenSutherlandClip(&a, &b, 0.0f, 0.0f, width - 1.0f, height - 1.0f)) {
#ifdef DEBUG
        printf("Line accepted from (%.2f; %.2f) to (%.2f; %.2f)\n", a.x, a.y,
               b.x, b.y);
#endif
        // DDA(a, b, color, frame_buffer, width);
        bresenham(a, b, color, frame_buffer, width);
        put_pixel((i32)roundf(a.x), (i32)roundf(a.y), COLOR_RED, frame_buffer,
                  width);
        put_pixel((i32)roundf(b.x), (i32)roundf(b.y), COLOR_WHITE, frame_buffer,
                  width);
    } else {
#ifdef DEBUG
        printf("Line completely rejected outside the window\n");
#endif
    }
}

static bool cohenSutherlandClip(Vector2* a, Vector2* b, f32 x_min, f32 y_min,
                                f32 x_max, f32 y_max) {
    // Compute code for a and b
    i32 code0 = compute_code(*a, x_min, y_min, x_max, y_max);
    i32 code1 = compute_code(*b, x_min, y_min, x_max, y_max);
    bool accepted = false;

    while (true) {
        if ((code0 == INSIDE) && (code1 == INSIDE)) {
            // Both points inside window so accept points
            accepted = true;
            break;
        } else if (code0 & code1) {
            // Both points share and outside zone so reject points
            break;
        } else {
            // Line needs clipping
            i32 code_out;
            f32 x;
            f32 y;

            // Select outside point
            if (code0 != INSIDE)
                code_out = code0;
            else
                code_out = code1;

            // Find intersection point using parametric line function
            if (code_out & TOP) {
                // Point is above the clip window
                x = a->x + (b->x - a->x) * (y_min - a->y) / (b->y - a->y);
                y = y_min;
            } else if (code_out & BOTTOM) {
                // Point is below the clip window
                x = a->x + (b->x - a->x) * (y_max - a->y) / (b->y - a->y);
                y = y_max;
            } else if (code_out & RIGHT) {
                // Point is to the right the clip window
                x = x_max;
                y = a->y + (b->y - a->y) * (x_max - a->x) / (b->x - a->x);
            } else if (code_out & LEFT) {
                // Point is to the left the clip window
                x = x_min;
                y = a->y + (b->y - a->y) * (x_min - a->x) / (b->x - a->x);
            }

            // Replace outside point with intersection point
            if (code_out == code0) {
                a->x = x;
                a->y = y;
                code0 = compute_code(*a, x_min, y_min, x_max, y_max);
            } else {
                b->x = x;
                b->y = y;
                code1 = compute_code(*b, x_min, y_min, x_max, y_max);
            }
        }
    }
    return accepted;
}

static i32 compute_code(Vector2 v, f32 x_min, f32 y_min, f32 x_max, f32 y_max) {
    i32 code = INSIDE;
    if (v.x < x_min)
        code |= LEFT;
    else if (v.x > x_max)
        code |= RIGHT;
    if (v.y < y_min)
        code |= TOP;
    else if (v.y > y_max)
        code |= BOTTOM;
    return code;
}

// static void DDA(Vector2 a, Vector2 b, Color color, Color* const frame_buffer,
//                 i32 width) {
//     i32 x1 = (i32)roundf(a.x);
//     i32 y1 = (i32)roundf(a.y);
//
//     i32 x2 = (i32)roundf(b.x);
//     i32 y2 = (i32)roundf(b.y);
//
//     i32 dx = x2 - x1;
//     i32 dy = y2 - y1;
//
//     i32 abs_dx = abs(dx);
//     i32 abs_dy = abs(dy);
//     i32 steps = abs_dx > abs_dy ? abs_dx : abs_dy;
//
//     if (steps == 0) {
//         put_pixel(x1, y1, color, frame_buffer, width);
//         return;
//     }
//
//     f32 x_inc = dx / (f32)steps;
//     f32 y_inc = dy / (f32)steps;
//
//     f32 x = (f32)x1;
//     f32 y = (f32)y1;
//
//     for (i32 i = 0; i <= steps; i++) {
//         put_pixel((i32)roundf(x), (i32)roundf(y), color, frame_buffer,
//         width); x += x_inc; y += y_inc;
//     }
// }

static void bresenham(Vector2 a, Vector2 b, Color color,
                      Color* const frame_buffer, i32 width) {
    i32 x1 = (i32)roundf(a.x);
    i32 y1 = (i32)roundf(a.y);

    i32 x2 = (i32)roundf(b.x);
    i32 y2 = (i32)roundf(b.y);

    i32 dx = abs(x2 - x1);
    i32 dy = abs(y2 - y1);

    i32 sx = x1 < x2 ? 1 : -1;
    i32 sy = y1 < y2 ? 1 : -1;

    i32 err = dx - dy;

    while (true) {
        put_pixel(x1, y1, color, frame_buffer, width);

        if (x1 == x2 && y1 == y2)
            break;

        i32 e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}
