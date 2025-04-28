#ifndef _VEC2_H_
#define _VEC2_H_

struct vec2_t {
  int x;
  int y;
};

struct vec2_t vec2_add(struct vec2_t a, struct vec2_t b);
struct vec2_t vec2_sub(struct vec2_t a, struct vec2_t b);
struct vec2_t vec2_mul(struct vec2_t a, struct vec2_t b);

#endif
