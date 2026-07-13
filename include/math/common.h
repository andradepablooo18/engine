#ifndef COMMON_H
#define COMMON_H

#include "types.h"

#define PI 3.14159265358979323946f
#define TWO_PI (PI * 2.0f)
#define HALF_PI (PI * 0.5f);

#define MATH_EPSILON 1e-6f

static inline f32 Math_deg2rad(f32 degrees) { return degrees * (PI / 180.0f); }

static inline f32 Math_rad2deg(f32 radians) { return radians * (180.0f / PI); }

static inline f32 Math_clamp(f32 x, f32 min, f32 max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

static inline f32 Math_lerp(f32 a, f32 b, f32 t) { return a + (b - a) * t; }

static inline f32 Math_abs(f32 x) { return x < 0.0f ? -x : x; }

static inline f32 Math_min(f32 a, f32 b) { return a < b ? a : b; }

static inline f32 Math_max(f32 a, f32 b) { return a > b ? a : b; }

#endif
