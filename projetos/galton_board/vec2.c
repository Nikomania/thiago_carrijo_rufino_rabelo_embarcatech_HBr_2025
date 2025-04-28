#include "vec2.h"

struct vec2_t vec2_add(struct vec2_t a, struct vec2_t b) {
    struct vec2_t result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

struct vec2_t vec2_sub(struct vec2_t a, struct vec2_t b) {
    struct vec2_t result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

struct vec2_t vec2_mul(struct vec2_t a, struct vec2_t b) {
    struct vec2_t result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}
