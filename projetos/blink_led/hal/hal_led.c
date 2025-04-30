#include "hal_led.h"
#include "led_embutido.h"
#include "hardware/watchdog.h"

void hal_led_init() {
    if (init_cyw43()) {
      watchdog_reboot(0, 0, 0); // reload raspberry, in case of error
    }
}

void hal_led_toggle() {
    static bool led_state = false;
    led_state = !led_state;
    gpio_set_pin_cyw43(CYW43_WL_GPIO_LED_PIN, led_state);
}
