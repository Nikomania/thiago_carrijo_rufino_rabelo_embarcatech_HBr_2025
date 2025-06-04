#ifndef _GPIO_H_
#define _GPIO_H_

#include "hardware/clocks.h"
#include "pico/stdlib.h"

#define BTN_A_PIN 5
#define BTN_B_PIN 6
#define RED_LED_PIN 13
#define GREEN_LED_PIN 11
// #define BLUE_LED_PIN 12

#define BTN_DEBOUNCE_MS 200 // Tempo de debounce dos botões em milissegundos.

struct button_state {
  absolute_time_t last_press_time;
  bool was_pressed;
  int btn_pin;
};

extern struct button_state btn_A_state;
extern struct button_state btn_B_state;

void gpio_callback(uint gpio, uint32_t event_mask);
void init_gpio();
bool get_button_state(struct button_state *btn_state);
void update_button_state(struct button_state *btn_state);

#endif
