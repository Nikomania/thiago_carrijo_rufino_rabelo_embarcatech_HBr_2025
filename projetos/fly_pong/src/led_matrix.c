#include "led_matrix.h"


COLORS led_matrix[LED_COUNT_X][LED_COUNT_Y] = {0};
PIO led_matrix_pio;
uint sm;

bool was_changed = false;

const rgb_t COLORS_ARRAY[COLORS_COUNT] = {
  {0, 0, 0},
  {16, 16, 16},
  {16, 0, 0},
  {0, 16, 0},
  {0, 0, 16},
  {16, 16, 0},
  {0, 16, 16},
  {16, 0, 16}
};

void initLedMatrix() {
  uint offset = pio_add_program(pio0, &ws2818b_program);
  led_matrix_pio = pio0;

  sm = pio_claim_unused_sm(led_matrix_pio, false);
  if (sm < 0) {  // sm is an unsigned int, therefore it can't be negative (how does this work?)
    led_matrix_pio = pio1;
    sm = pio_claim_unused_sm(led_matrix_pio, true);
  }

  ws2818b_program_init(led_matrix_pio, sm, offset, LED_MATRIX_PIN, 800000.f);

  clearLEDs();
}

void clearLEDs() {
  for (uint8_t i = 0; i < LED_COUNT_X; i++)
    for (uint8_t j = 0; j < LED_COUNT_Y; j++)
      setLED(i, j, BLACK);
}

void setLED(const uint index_x, const uint index_y, const COLORS color) {
  if (index_x >= LED_COUNT_X || index_y >= LED_COUNT_Y) {
    return;
  }
  was_changed = true;

  led_matrix[index_x][index_y] = color;
}

void setLEDs(const COLORS leds[LED_COUNT_X][LED_COUNT_Y]) {
  for (uint8_t i = 0; i < LED_COUNT_X; i++)
    for (uint8_t j = 0; j < LED_COUNT_Y; j++)
      setLED(i, j, leds[i][j]);
}

void clear() {
  for (uint8_t i = 0; i < LED_COUNT_X; i++)
    for (uint8_t j = 0; j < LED_COUNT_Y; j++)
    led_matrix[i][j] = BLACK;
}

void render() {
  if (!was_changed) {
    return;
  }
  was_changed = false;
  for (uint8_t j = 0; j < LED_COUNT_Y; j++) {
    for (int8_t i = 0; i < LED_COUNT_X; i++) {
      pio_sm_put_blocking(
        led_matrix_pio,
        sm,
        COLORS_ARRAY[led_matrix[j % 2 == 0 ? LED_COUNT_X - 1 - i : i][j]].G
      );
      pio_sm_put_blocking(
        led_matrix_pio,
        sm,
        COLORS_ARRAY[led_matrix[j % 2 == 0 ? LED_COUNT_X - 1 - i : i][j]].R
      );
      pio_sm_put_blocking(
        led_matrix_pio,
        sm,
        COLORS_ARRAY[led_matrix[j % 2 == 0 ? LED_COUNT_X - 1 - i : i][j]].B
      );
    }
  }
  sleep_us(100); // wait for 100us, RESET signal from datasheet
}

// setLED(0, 4, color); setLED(1, 4, color); setLED(2, 4, color); setLED(3, 4, color); setLED(4, 4, color);
// setLED(0, 3, color); setLED(1, 3, color); setLED(2, 3, color); setLED(3, 3, color); setLED(4, 3, color);
// setLED(0, 2, color); setLED(1, 2, color); setLED(2, 2, color); setLED(3, 2, color); setLED(4, 2, color);
// setLED(0, 1, color); setLED(1, 1, color); setLED(2, 1, color); setLED(3, 1, color); setLED(4, 1, color);
// setLED(0, 0, color); setLED(1, 0, color); setLED(2, 0, color); setLED(3, 0, color); setLED(4, 0, color);


void setSmile(COLORS color) {
  clear();
                       setLED(1, 4, color);                      setLED(3, 4, color);
                       setLED(1, 3, color);                      setLED(3, 3, color);

  setLED(0, 1, color);                                                                setLED(4, 1, color);
  setLED(0, 0, color); setLED(1, 0, color); setLED(2, 0, color); setLED(3, 0, color); setLED(4, 0, color);
}

void setCheck(COLORS color) {
  clear();
                                                                                      setLED(4, 3, color);
                                                                 setLED(3, 2, color);
  setLED(0, 1, color);                      setLED(2, 1, color);
                       setLED(1, 0, color);
}

void setCross(COLORS color) {
  clear();
  setLED(0, 4, color);                                                                setLED(4, 4, color);
                       setLED(1, 3, color);                      setLED(3, 3, color);
                                            setLED(2, 2, color);
                       setLED(1, 1, color);                      setLED(3, 1, color);
  setLED(0, 0, color);                                                                setLED(4, 0, color);
}

void setNumber(const uint8_t number, COLORS color) {
  switch (number) {
    case 0:
      setNumberZero(color);
      break;
    case 1:
      setNumberOne(color);
      break;
    case 2:
      setNumberTwo(color);
      break;
    case 3:
      setNumberThree(color);
      break;
    case 4:
      setNumberFour(color);
      break;
    case 5:
      setNumberFive(color);
      break;
    case 6:
      setNumberSix(color);
      break;
    case 7:
      setNumberSeven(color);
      break;
    case 8:
      setNumberEight(color);
      break;
    case 9:
      setNumberNine(color);
      break;
    default:
      break;
  }
}

void setNumberZero(COLORS color) {
    clear();
    setLED(1, 4, color); setLED(2, 4, color); setLED(3, 4, color);
    setLED(1, 3, color);                      setLED(3, 3, color);
    setLED(1, 2, color);                      setLED(3, 2, color);
    setLED(1, 1, color);                      setLED(3, 1, color);
    setLED(1, 0, color); setLED(2, 0, color); setLED(3, 0, color);
}

void setNumberOne(COLORS color) {
  clear();
                       setLED(2, 4, color);
  setLED(1, 3, color); setLED(2, 3, color);
                       setLED(2, 2, color);
                       setLED(2, 1, color);
  setLED(1, 0, color); setLED(2, 0, color); setLED(3, 0, color);
  render();
}

void setNumberTwo(COLORS color) {
  clear();
  setLED(1, 4, color); setLED(2, 4, color); setLED(3, 4, color);
                                            setLED(3, 3, color);
  setLED(1, 2, color); setLED(2, 2, color); setLED(3, 2, color);
  setLED(1, 1, color);
  setLED(1, 0, color); setLED(2, 0, color); setLED(3, 0, color);
  render();
}

void setNumberThree(COLORS color) {
    clear();
    setLED(1, 4, color); setLED(2, 4, color); setLED(3, 4, color);
                                              setLED(3, 3, color);
    setLED(1, 2, color); setLED(2, 2, color); setLED(3, 2, color);
                                              setLED(3, 1, color);
    setLED(1, 0, color); setLED(2, 0, color); setLED(3, 0, color);
}

void setNumberFour(COLORS color) {
    clear();
    setLED(1, 4, color);                      setLED(3, 4, color);
    setLED(1, 3, color);                      setLED(3, 3, color);
    setLED(1, 2, color); setLED(2, 2, color); setLED(3, 2, color);
                                              setLED(3, 1, color);
                                              setLED(3, 0, color);
}

void setNumberFive(COLORS color) {
    clear();
    setLED(1, 4, color); setLED(2, 4, color); setLED(3, 4, color);
    setLED(1, 3, color);
    setLED(1, 2, color); setLED(2, 2, color); setLED(3, 2, color);
                                              setLED(3, 1, color);
    setLED(1, 0, color); setLED(2, 0, color); setLED(3, 0, color);
}

void setNumberSix(COLORS color) {
    clear();
    setLED(1, 4, color); setLED(2, 4, color); setLED(3, 4, color);
    setLED(1, 3, color);
    setLED(1, 2, color); setLED(2, 2, color); setLED(3, 2, color);
    setLED(1, 1, color);                      setLED(3, 1, color);
    setLED(1, 0, color); setLED(2, 0, color); setLED(3, 0, color);
}

void setNumberSeven(COLORS color) {
    clear();
    setLED(1, 4, color); setLED(2, 4, color); setLED(3, 4, color);
                                              setLED(3, 3, color);
                                              setLED(3, 2, color);
                                              setLED(3, 1, color);
                                              setLED(3, 0, color);
}

void setNumberEight(COLORS color) {
    clear();
    setLED(1, 4, color); setLED(2, 4, color); setLED(3, 4, color);
    setLED(1, 3, color);                      setLED(3, 3, color);
    setLED(1, 2, color); setLED(2, 2, color); setLED(3, 2, color);
    setLED(1, 1, color);                      setLED(3, 1, color);
    setLED(1, 0, color); setLED(2, 0, color); setLED(3, 0, color);
}

void setNumberNine(COLORS color) {
    clear();
    setLED(1, 4, color); setLED(2, 4, color); setLED(3, 4, color);
    setLED(1, 3, color);                      setLED(3, 3, color);
    setLED(1, 2, color); setLED(2, 2, color); setLED(3, 2, color);
                                              setLED(3, 1, color);
    setLED(1, 0, color); setLED(2, 0, color); setLED(3, 0, color);
}
