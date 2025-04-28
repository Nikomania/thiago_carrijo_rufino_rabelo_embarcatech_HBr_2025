#ifndef _MAP_H_
#define _MAP_H_

#include "oled.h"

#define GAP_LINES 2
#define GAP_POINTS 2

#define TOTAL_LINES (int) (ssd1306_height - 1) / ((GAP_LINES + 1)) + 1 // not considering some lines that were removed
#define LAST_LINE_INDEX (TOTAL_LINES - 1) * (GAP_LINES + 1)
#define IS_LAST_LINE_DISLOCATED (int) (LAST_LINE_INDEX) % (2 * (GAP_LINES + 1)) == GAP_LINES + 1
#define TOTAL_GAP_BETWEEN_PIN_LAST_LINE (int) (ssd1306_width - 1 - (IS_LAST_LINE_DISLOCATED ? 0 : GAP_POINTS + 1)) / (2 * (GAP_POINTS + 1)) + 1
#define TOTAL_GAP_BETWEEN_PIN_PER_LINE (int) (ssd1306_width - 1) / (2 * (GAP_POINTS + 1)) + 1
#define INDEX_MIDDLE_GAP (int) (TOTAL_GAP_BETWEEN_PIN_PER_LINE - 1) / 2
#define COL_MIDDLE_GAP INDEX_MIDDLE_GAP * (2 * (GAP_POINTS + 1))

void draw_map();

bool is_between_pin(int x0, int y0);
bool is_pin(int x0, int y0);
bool is_pin_removed(int x0, int y0);

int get_gap_index(int x);
int get_line_index(int y);

#endif
