#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "pico/rand.h"
#include "oled.h"
#include "vec2.h"
#include "ball.h"
#include "map.h"

#define tick_ms 50

#define BALL_COUNT_DISPLAY_TICKS 100
// #define SHOW_PERCENTAGE


struct ball_t balls[BALL_PER_SIMULATION];
int ball_histogram[TOTAL_GAP_BETWEEN_PIN_LAST_LINE];
int balls_count = 0;

volatile int tick_count = 0;
bool tick_callback(__unused struct repeating_timer *t);


enum state_t {
    STATE_SIMULATION,
    STATE_COUNTDOWN
};
enum state_t state = STATE_SIMULATION;
void update_state(enum state_t *state);
void update_simulation();
void update_countdown();


int rand_0_1();

void center_c_str(char *str, int length);
void get_c_string_from_int(int n, char *str);
int count_digits_10(int n);


int main() {
    stdio_init_all();
    
    init_OLed();
    clear_OLed();

    draw_map();

    balls_init(balls, ssd, BALL_X0, BALL_Y0, tick_count);

    render_OLed();

    for (int i = 0; i < TOTAL_GAP_BETWEEN_PIN_LAST_LINE; i++) {
        ball_histogram[i] = 0;
    }

    #ifndef SHOW_PERCENTAGE
    struct repeating_timer timer;
    add_repeating_timer_ms(tick_ms, tick_callback, NULL, &timer);
    #endif

    int current_tick = tick_count;
    while(true) {
        if (current_tick <= tick_count) {
            update_state(&state);
    
            render_OLed();
            current_tick++;
        }
        #ifdef SHOW_PERCENTAGE
        sleep_ms(tick_ms);
        tick_count++;
        #endif
    }

    return 0;
}

int rand_0_1() {
    return (int)(get_rand_32() & 1);
}

int count_digits_10(int n) {
    if (n == 0) {
        return 1;
    }
    
    return (int)(floor(log10(n))+1);
}

bool tick_callback(__unused struct repeating_timer *t) {
    tick_count++;
    return true;
}

void update_state(enum state_t* state) {
    switch (*state) {
        case STATE_SIMULATION:
            update_simulation();
            break;
        case STATE_COUNTDOWN:
            update_countdown();
            break;
    }
}

void update_simulation() {
    volatile int balls_finished = 0;
    for (int i = 0; i < BALL_PER_SIMULATION; i++) {
        if (balls[i].tick_to_unlock > tick_count) {
            continue;
        }
        if (balls[i].finished) {
            balls_finished++;
            continue;
        }
        ball_update(balls + i);
        ball_move(balls + i, 0, 1);
    
        if (balls[i].curr_pos.y >= ssd1306_height) {
            balls[i].finished = true;
            ball_histogram[get_gap_index(balls[i].curr_pos.x)]++;
            ball_undraw(balls + i, ssd);
            balls_finished++;
            continue;
        }
    
        if (is_pin(balls[i].curr_pos.x, balls[i].curr_pos.y)) {
            int dir = rand_0_1();
            // 0 = left, 1 = right
            int plus_or_minus = (dir * 2 - 1);
            ball_move(balls + i, plus_or_minus * (GAP_POINTS + 1), 0);
        }
        ball_draw(balls + i, ssd);
    }
    if (balls_finished == BALL_PER_SIMULATION) {
        state = STATE_COUNTDOWN;
        balls_count += BALL_PER_SIMULATION;
        clear_OLed();
    }
}

void update_countdown() {
    static int countdown_ticks = 0;
    if (countdown_ticks == tick_count) {
        state = STATE_SIMULATION;
        clear_OLed();
        draw_map();
        balls_init(balls, ssd, BALL_X0, BALL_Y0, tick_count);
        return;
    }
    if (countdown_ticks < tick_count) {
        countdown_ticks = tick_count + BALL_COUNT_DISPLAY_TICKS + 1;
        return;
    }

    char ball_count_str[17] = "               ";
    sprintf(ball_count_str, "%16d", balls_count);
    // get_c_string_from_int(balls_count, ball_count_str);
    // center_c_str(ball_count_str, count_digits_10(balls_count) + 1);

    const unsigned int n_lines = 1;
    char* o_led_text[] = {
        ball_count_str
    };
    print_lines_OLed(o_led_text, n_lines, 0, 0);

    for (int i = 0; i < TOTAL_GAP_BETWEEN_PIN_LAST_LINE; i++) {
        int x = i * 2 * (GAP_POINTS + 1) + (IS_LAST_LINE_DISLOCATED ? 0 : GAP_POINTS + 1);
        #ifdef SHOW_PERCENTAGE
        draw_line_OLed(x, ssd1306_height - 1, x, floor((100 - ball_histogram[i] * 100 / balls_count) * (ssd1306_height - 1) / 100), true);
        #endif
        draw_line_OLed(x, ssd1306_height - 1, x, ssd1306_height - 1 - (ball_histogram[i] < ssd1306_height ? ball_histogram[i] : ssd1306_height - 1), true);
    }
}
