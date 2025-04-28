#ifndef _BALL_H_
#define _BALL_H_

#include "vec2.h"
#include "oled.h"
#include "map.h"
#include <stdio.h>
#include <stdint.h>

#define BALL_X0 COL_MIDDLE_GAP
#define BALL_Y0 0
#define BALL_PER_SIMULATION 200
#define TICKS_BETWEEN_BALLS 3

struct ball_t {
  struct vec2_t curr_pos;
  struct vec2_t prev_pos;
  int tick_to_unlock;
  bool finished;
};

void balls_init(struct ball_t* balls, uint8_t* ssd, int x, int y, int tick_count);
void ball_init(struct ball_t* ball, int x, int y, int tick_to_unlock);
void ball_update(struct ball_t* ball);
void ball_set_pos(struct ball_t* ball, int x, int y);
void ball_move(struct ball_t* ball, int x, int y);
bool ball_is_there(struct ball_t* ball, int x, int y);
void ball_draw(struct ball_t* ball, uint8_t* ssd);
void ball_undraw(struct ball_t* ball, uint8_t* ssd);

#endif
