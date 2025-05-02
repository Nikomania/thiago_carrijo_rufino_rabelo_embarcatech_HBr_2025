#ifndef OLED_H
#define OLED_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "ssd1306.h"
#include "hardware/i2c.h"

#define MAX_LINES 8
#define CHAR_HEIGHT 8
#define MAX_CHAR 16
#define I2C_SDA 14
#define I2C_SCL 15

// ssd1306_width pixels por ssd1306_n_pages páginas
extern struct render_area frame_area;

extern uint8_t ssd[ssd1306_buffer_length];

void init_OLed();

void print_lines_OLed(char **text, unsigned int n_lines, int x0, int y0);
void clear_OLed();

#endif
