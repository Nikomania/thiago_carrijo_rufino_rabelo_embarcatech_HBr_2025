#include "map.h"
#include "ball.h"
#include <math.h>

// void create_map() {
//   for (int i = 0; i < ssd1306_height; i++) {
//     for (int j = 0; j < ssd1306_width; j++) {
//       map[i][j] = false;
//     }
//   }

//   int alternate = 0;
//   for (int i = GAP_LINES + 1; i < ssd1306_height; i += 1 + GAP_LINES) {
//     for (int j = 0; j < ssd1306_width; j += 1 + GAP_POINTS) {
//       if (j % 2 == alternate)
//         map[i][j] = true;
//     }
//     alternate = alternate == 0 ? 1 : 0;
//   }
// }
const int PINS_REMOVED = 15;

int pins_removed[][2] = {
  {INDEX_MIDDLE_GAP + 1, 3},
  {INDEX_MIDDLE_GAP + 1, 4},
  {INDEX_MIDDLE_GAP + 1, 5},
  {INDEX_MIDDLE_GAP - 1, 6},
  {INDEX_MIDDLE_GAP - 1, 7},
  {INDEX_MIDDLE_GAP - 1, 8},
  {INDEX_MIDDLE_GAP - 2, 9},
  {INDEX_MIDDLE_GAP + 1, 10},
  {INDEX_MIDDLE_GAP - 2, 11},
  {INDEX_MIDDLE_GAP - 1, 12},
  {INDEX_MIDDLE_GAP + 1, 13},
  {INDEX_MIDDLE_GAP - 1, 14},
  {INDEX_MIDDLE_GAP - 2, 15},
  {INDEX_MIDDLE_GAP - 1, 16},
  {INDEX_MIDDLE_GAP - 3, 17},
};

void draw_map() {
  for (int i = 0; i < ssd1306_height; i++) {
    for (int j = 0; j < ssd1306_width; j++) {
      draw_point_OLed(j, i, is_pin(j, i));
    }
  }
}

bool is_pin(int x0, int y0) {
  // y0 < GAP_LINES + 1 so that the first line is not considered (ball starts at 0)
  if (x0 < 0 || x0 >= ssd1306_width || y0 < GAP_LINES + 1 || y0 >= ssd1306_height) {
    return false;
  }
  bool is_line_with_pin = (y0 % (GAP_LINES + 1) == 0);
  bool is_line_dislocated = (y0 % (2 * (GAP_LINES + 1)) == GAP_LINES + 1);
  int col_with_pin = is_line_dislocated ? GAP_POINTS + 1 : 0;
  bool is_col_with_pin = (x0 % (2 * (GAP_POINTS + 1)) == col_with_pin);
  if (!is_line_with_pin || !is_col_with_pin || is_pin_removed(x0, y0)) {
    return false;
  }
  return true;
}

bool is_between_pin(int x0, int y0) {
  // y0 < GAP_LINES + 1 so that the first line is not considered (ball starts at 0)
  if (x0 < 0 || x0 >= ssd1306_width || y0 < GAP_LINES + 1 || y0 >= ssd1306_height) {
    return false;
  }
  
  bool is_line_with_pin = (y0 % (GAP_LINES + 1) == 0);
  bool is_line_dislocated = (y0 % (2 * (GAP_LINES + 1)) == GAP_LINES + 1);
  int col_between_pin = !is_line_dislocated ? GAP_POINTS + 1 : 0;
  bool is_col_between_pin = (x0 % (2 * (GAP_POINTS + 1)) == col_between_pin);
  if (!is_line_with_pin || !is_col_between_pin) {
    return false;
  }
  return true;
}

bool is_pin_removed(int x0, int y0) {
  bool is_line_dislocated = (y0 % (2 * (GAP_LINES + 1)) == GAP_LINES + 1);
  for (int i = 0; i < PINS_REMOVED; i++) {
    if (pins_removed[i][0] == floor((x0 - (is_line_dislocated ? GAP_POINTS + 1 : 0 )) / (2 * (GAP_POINTS + 1))) && pins_removed[i][1] == get_line_index(y0)) {
      return true;
    }
  }
  return false;
}

int get_gap_index(int x) {
  return floor(x / (2 * (GAP_POINTS + 1)));
}

int get_line_index(int y) {
  return floor(y / (GAP_LINES + 1));
}
