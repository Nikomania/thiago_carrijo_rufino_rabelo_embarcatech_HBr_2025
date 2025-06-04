#include "gpio.h"

struct button_state btn_A_state = {0, false, BTN_A_PIN};
struct button_state btn_B_state = {0, false, BTN_B_PIN};

void init_gpio() {
  gpio_init(BTN_A_PIN);
  gpio_set_dir(BTN_A_PIN, GPIO_IN);
  gpio_pull_up(BTN_A_PIN);

  gpio_init(BTN_B_PIN);
  gpio_set_dir(BTN_B_PIN, GPIO_IN);
  gpio_pull_up(BTN_B_PIN);

  gpio_init(RED_LED_PIN);
  gpio_set_dir(RED_LED_PIN, GPIO_OUT);
  gpio_put(RED_LED_PIN, 0); // Desliga o LED vermelho.

  gpio_init(GREEN_LED_PIN);
  gpio_set_dir(GREEN_LED_PIN, GPIO_OUT);
  gpio_put(GREEN_LED_PIN, 0); // Desliga o LED verde.

  gpio_set_irq_enabled_with_callback(BTN_A_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
  gpio_set_irq_enabled_with_callback(BTN_B_PIN, GPIO_IRQ_EDGE_FALL, true, gpio_callback);
}

bool get_button_state(struct button_state *btn_state) {
  bool state = btn_state->was_pressed;
  btn_state->was_pressed = false;
  return state;
}

void update_button_state(struct button_state *btn_state) {
  if (
    !btn_state->was_pressed &&
    absolute_time_diff_us(btn_state->last_press_time, get_absolute_time()) > BTN_DEBOUNCE_MS * 1000
  ) {
    btn_state->last_press_time = get_absolute_time();
    btn_state->was_pressed = true;
  }
}

void gpio_callback(uint gpio, uint32_t event_mask) {
  switch (gpio) {
      case BTN_A_PIN:
          update_button_state(&btn_A_state);
          break;
      case BTN_B_PIN:
          update_button_state(&btn_B_state);
          break;
  }
}
