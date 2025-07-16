#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTON_PIN 10 // Define the GPIO pin for the button

void gpio_setup(void);
bool is_button_pressed(void);

#endif
