#include "Rasterizer2D.h"
#include "RasterizerCommon.h"
#include "SDL3/SDL.h"
#include "core/Color.h"
#include "math/common.h"
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

static void draw_line(Vector2 a, Vector2 b, Color color, Color* frame_buffer,
                      i32 width);
static bool clip_line_to_framebuffer(Vector2* a, Vector2* b, i32 width,
                                     i32 height);
static bool cohenSutherlandClip(Vector2* a, Vector2* b, f32 x_min, f32 y_min,
                                f32 x_max, f32 y_max);
static i32 compute_code(Vector2 v, f32 x_min, f32 y_min, f32 x_max, f32 y_max);
// static void DDA(Vector2 a, Vector2 b, Color color, Color* frame_buffer,
//                 i32 width);
static void bresenham(Vector2 a, Vector2 b, Color color, Color* frame_buffer,
                      i32 width);

void Rasterizer2D_draw_pixel(i32 x, i32 y, Color color, Color* frame_buffer,
                             i32 width, i32 height) {
    assert(frame_buffer);
    assert(x >= 0 && x < width);
    assert(y >= 0 && y < height);
    frame_buffer[y * width + x] = color;
}

void Rasterizer2D_draw_line(Vector2 a, Vector2 b, Color color,
                            Color* frame_buffer, i32 width, i32 height) {
    assert(frame_buffer);
    /* Check if line was accepted (even if it was clipped) in order to be drawn,
     * or rejected in case it is outside the buffer */
    if (clip_line_to_framebuffer(&a, &b, width, height)) {
        // DDA(a, b, color, frame_buffer, width);
        bresenham(a, b, color, frame_buffer, width);
    }
}

static void draw_line(Vector2 a, Vector2 b, Color color, Color* frame_buffer,
                      i32 width) {
    // DDA(a, b, color, frame_buffer, width);
    bresenham(a, b, color, frame_buffer, width);
}

static bool clip_line_to_framebuffer(Vector2* a, Vector2* b, i32 width,
                                     i32 height) {
    return cohenSutherlandClip(a, b, 0.0f, 0.0f, width - 1.0f, height - 1.0f);
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

// static void DDA(Vector2 a, Vector2 b, Color color, Color* frame_buffer,
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

static void bresenham(Vector2 a, Vector2 b, Color color, Color* frame_buffer,
                      i32 width) {
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

void Rasterizer2D_draw_triangle_solid(Vector2 a, Vector2 b, Vector2 c,
                                      Color* frame_buffer, i32 width,
                                      i32 height) {
    Color colors[3] = {COLOR_RED, COLOR_GREEN, COLOR_BLUE};

    // Find bounding box with all the candidate pixels
    i32 x_min = floor(Math_min(a.x, (Math_min(b.x, c.x))));
    i32 y_min = floor(Math_min(a.y, Math_min(b.y, c.y)));
    i32 x_max = ceil(Math_max(a.x, (Math_max(b.x, c.x))));
    i32 y_max = ceil(Math_max(a.y, Math_max(b.y, c.y)));
    // Check boundaries and clip triangle if necessary
    x_min = Math_max(x_min, 0);
    y_min = Math_max(y_min, 0);
    x_max = Math_min(x_max, width - 1);
    y_max = Math_min(y_max, height - 1);

    // Compute the area of the parallelogram
    f32 area = edge(a, b, c);

    // Stick to top-left rule filling convention
    f32 bias0 = is_top_left(b, c) ? 0 : -0.0001;
    f32 bias1 = is_top_left(c, a) ? 0 : -0.0001;
    f32 bias2 = is_top_left(a, b) ? 0 : -0.0001;

    // Compute the constant delta values that will be used for horizontal and
    // vertical steps in order to avoid computing edge function each
    // iteration
    f32 delta_w0_col = b.y - c.y;
    f32 delta_w1_col = c.y - a.y;
    f32 delta_w2_col = a.y - b.y;

    f32 delta_w0_row = c.x - b.x;
    f32 delta_w1_row = a.x - c.x;
    f32 delta_w2_row = b.x - a.x;

    // Compute edge function to see if pixel is inside triangle
    Vector2 p = {x_min + 0.5f, y_min + 0.5f};
    f32 w0_row = edge(b, c, p) + bias0;
    f32 w1_row = edge(c, a, p) + bias1;
    f32 w2_row = edge(a, b, p) + bias2;

    bool is_inside;

    // Loop all candidate pixels inside the bounding box
    for (i32 y = y_min; y <= y_max; y++) {
        f32 w0 = w0_row;
        f32 w1 = w1_row;
        f32 w2 = w2_row;
        for (i32 x = x_min; x <= x_max; x++) {
            if (area >= 0.0f) {
                is_inside = w0 >= 0 && w1 >= 0 && w2 >= 0;
            } else {
                is_inside = w0 <= 0 && w1 <= 0 && w2 <= 0;
            }
            if (is_inside) {
                // Compute barycentric coordinates alpha, beta and gamma
                f32 alpha = w0 / area;
                f32 beta = w1 / area;
                f32 gamma = w2 / area;
                u8 r = alpha * Color_get_red(colors[0]) +
                       beta * Color_get_red(colors[1]) +
                       gamma * Color_get_red(colors[2]);
                u8 g = alpha * Color_get_green(colors[0]) +
                       beta * Color_get_green(colors[1]) +
                       gamma * Color_get_green(colors[2]);
                u8 b = alpha * Color_get_blue(colors[0]) +
                       beta * Color_get_blue(colors[1]) +
                       gamma * Color_get_blue(colors[2]);
                put_pixel(x, y, Color_create(r, g, b, 0xFF), frame_buffer,
                          width);
            }
            w0 += delta_w0_col;
            w1 += delta_w1_col;
            w2 += delta_w2_col;
        }
        w0_row += delta_w0_row;
        w1_row += delta_w1_row;
        w2_row += delta_w2_row;
    }
}
