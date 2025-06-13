#include "button.h"
#include "led_rgb.h"
#include "buzzer.h"

TaskHandle_t xLEDTaskHandle = NULL;
TaskHandle_t xBuzzerTaskHandle = NULL;

struct btn_state btn_a = {BTN_A, BTN_RELEASED, 0, &xLEDTaskHandle};
struct btn_state btn_b = {BTN_B, BTN_RELEASED, 0, &xBuzzerTaskHandle};

void init_button(uint pin) {
    gpio_init(pin);
    gpio_set_dir(pin, GPIO_IN);
    gpio_pull_up(pin);
    
    if (pin == BTN_A) {
        xTaskCreate(vTaskButton, "Button A Task", configMINIMAL_STACK_SIZE, &btn_a, 3, NULL);
    } else if (pin == BTN_B) {
        xTaskCreate(vTaskButton, "Button B Task", configMINIMAL_STACK_SIZE, &btn_b, 3, NULL);
    }
}

void vTaskButton(void *pvParameters) {
    struct btn_state *btn = (struct btn_state *)pvParameters;
    ButtonState new_state;
    
    while (true) {
        bool current_pin_state = gpio_get(btn->pin) == 0;
        new_state = current_pin_state ? BTN_PRESSED : BTN_RELEASED;
        
        if (new_state != btn->state) {
            uint32_t now = to_ms_since_boot(get_absolute_time());
            if (now - btn->last_change_time > BTN_DEBOUNCE) {
                btn->state = new_state;
                btn->last_change_time = now;
                
                // Ação baseada no novo estado
                if (*(btn->task_handle) != NULL) {
                    if (btn->state == BTN_PRESSED) {
                        vTaskSuspend(*(btn->task_handle)); // Suspende ao pressionar
                    } else {
                        vTaskResume(*(btn->task_handle));  // Retoma ao soltar
                    }
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100)); // Polling a cada 100ms
    }
}