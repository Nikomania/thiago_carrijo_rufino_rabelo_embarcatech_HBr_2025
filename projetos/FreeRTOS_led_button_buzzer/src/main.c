#include "buzzer.h"
#include "led_rgb.h"
#include "button.h"
#include "FreeRTOS.h"
#include "task.h"

void vTaskLED(void *pvParameters) {
    int acc = 1;
    while (true) {
        switch (acc) {
        case 1:
            set_led_color(true, false, false); // Vermelho
            break;
        case 2:
            set_led_color(false, true, false); // Verde
            break;
        case 3:
            set_led_color(false, false, true); // Azul
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(500));
        acc = (acc % 3) + 1;
    }
}

void vTaskBuzzer(void *pvParameters) {
    while (true) {
        beep(BUZZER_PIN, 100);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    pwm_init_buzzer(BUZZER_PIN);
    init_led_rgb(RED_PIN, GREEN_PIN, BLUE_PIN);

    // Cria tarefas primeiro (handles serão configurados)
    xTaskCreate(vTaskLED, "LED Task", configMINIMAL_STACK_SIZE, NULL, 2, &xLEDTaskHandle);
    xTaskCreate(vTaskBuzzer, "Buzzer Task", configMINIMAL_STACK_SIZE, NULL, 1, &xBuzzerTaskHandle);

    // Depois inicializa botões (agora com handles válidos)
    init_button(BTN_A);
    init_button(BTN_B);

    vTaskStartScheduler();

    while (true) {
        tight_loop_contents();
    }
}