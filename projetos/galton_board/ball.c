#include "ball.h"

void balls_init(struct ball_t *balls, uint8_t* ssd, int x, int y, int tick_count) {
    for (int i = 0; i < BALL_PER_SIMULATION; i++) {
        ball_init(balls + i, BALL_X0, BALL_Y0, tick_count + i * TICKS_BETWEEN_BALLS);
        ball_draw(balls + i, ssd);
    }
}

void ball_init(struct ball_t* ball, int x, int y, int tick_to_unlock) { 
    ball->curr_pos.x = x;
    ball->curr_pos.y = y;
    ball->prev_pos.x = x;
    ball->prev_pos.y = y;
    ball->tick_to_unlock = tick_to_unlock;
    ball->finished = false;
}

void ball_update(struct ball_t* ball) {
    ball->prev_pos.x = ball->curr_pos.x;
    ball->prev_pos.y = ball->curr_pos.y;
}

void ball_set_pos(struct ball_t* ball, int x, int y) {
    ball->curr_pos.x = x;
    ball->curr_pos.y = y;
}

void ball_move(struct ball_t* ball, int x, int y) {
    ball->curr_pos.x += x;
    ball->curr_pos.y += y;
}

bool ball_is_there(struct ball_t* ball, int x, int y) {
    return (ball->curr_pos.x == x && ball->curr_pos.y == y);
}

void ball_draw(struct ball_t* ball, uint8_t* ssd) {
    draw_point_OLed(ball->prev_pos.x, ball->prev_pos.y, false);
    draw_point_OLed(ball->curr_pos.x, ball->curr_pos.y, true);
}

void ball_undraw(struct ball_t* ball, uint8_t* ssd) {
    draw_point_OLed(ball->prev_pos.x, ball->prev_pos.y, false);
}
