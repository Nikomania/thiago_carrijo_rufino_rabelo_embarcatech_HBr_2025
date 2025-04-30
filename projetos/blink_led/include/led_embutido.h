#ifndef _LED_EMBUTIDO_H_
#define _LED_EMBUTIDO_H_

#include "pico/stdlib.h"

int init_cyw43();
void gpio_set_pin_cyw43(int pin, bool bit);

#endif
