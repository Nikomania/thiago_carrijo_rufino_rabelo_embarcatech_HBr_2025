#include "gpio.h"

void gpio_setup(void) {
    // Initialize the GPIO for the button
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN); // Enable pull-up resistor
}

bool is_button_pressed(void) {
    // Read the state of the button
    return !gpio_get(BUTTON_PIN); // Return true if pressed (active low)
}
