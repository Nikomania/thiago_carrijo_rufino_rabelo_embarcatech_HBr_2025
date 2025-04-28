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

#define tick_ms 5

#define BALL_X_X0 0

bool tick = false;

// void tick_callback(__unused struct repeating_timer *t) {
//     tick = true;
// }

int rand_0_1();
void center_c_str(char *str, int length);
void get_c_string_from_int(int n, char *str);
int count_digits_10(int n);

int main() {
    stdio_init_all();
    
    init_OLed();
    clear_OLed();

    // create_map();
    // draw_map();

    // ball_init(&ball, 0, 0);

    // render_OLed();

    // struct repeating_timer timer;
    // add_repeating_timer_ms(tick_ms, tick_callback, NULL, &timer);

    int count_1 = 0;
    int count_0 = 0;
    while(true) {
        // ball_update(&ball);
        char rand_str[17] = "                ";
        int rand_n = rand_0_1();
        get_c_string_from_int(rand_n, rand_str);
        center_c_str(rand_str, count_digits_10(rand_n) + 1);

        if (rand_n == 0) {
            count_0++;
        } else {
            count_1++;
        }

        char count0_str[17] = "                ";
        get_c_string_from_int(count_0, count0_str);
        center_c_str(count0_str, count_digits_10(count_0) + 1);

        char count1_str[17] = "                ";
        get_c_string_from_int(count_1, count1_str);
        center_c_str(count1_str, count_digits_10(count_1) + 1);

        const unsigned int n_lines = 8;
        char* text[] = {
            "AAAAAAAAAAAAAAAA",
            "CURRENT NUMBER",
            rand_str,
            "COUNT 0",
            count0_str,
            "COUNT 1",
            count1_str,
            "AAAAAAAAAAAAAAAA",
        };
        print_lines_OLed(text, n_lines, 0, 0);

        render_OLed();
        tick = false;
        sleep_ms(tick_ms);
        clear_OLed();
    }

    return 0;
}

int rand_0_1() {
    return (int)(get_rand_32() & 1);
}

void center_c_str(char* str, int length) {
    const int white_spaces = ((1 + MAX_CHAR - length) >> 1);
    if (white_spaces <= 0) {
        return;
    }
    for (int i = length; i >= 0; i--) {
        str[i + white_spaces] = str[i];
        str[i] = ' ';
    }
}

void get_c_string_from_int(int n, char* str) {
    const int length = count_digits_10(n);
    for (int i = length - 1; i >= 0; i--) {
        str[i] = (n % 10) + '0';
        n /= 10;
    }
}

int count_digits_10(int n) {
    if (n == 0) {
        return 1;
    }
    
    return (int)(floor(log10(n))+1);
}
