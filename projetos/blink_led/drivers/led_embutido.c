#include "led_embutido.h"
#include "pico/cyw43_arch.h"

int init_cyw43() {
  return cyw43_arch_init();
}

void gpio_set_pin_cyw43(int pin, bool bit) {
  cyw43_arch_gpio_put(pin, bit);
}
