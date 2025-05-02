#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "temperature.h"
#include "oled.h"

#define ADC_TEMPERATURE_CHANNEL 4

#define UPDATE_PERIOD 1000 // ms

void center_c_str(char *str);
void get_c_string_from_int(int n, char *str);
void get_c_string_from_float(float n, char *str);

int main() {
    stdio_init_all();

    init_OLed();
    clear_OLed();

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(ADC_TEMPERATURE_CHANNEL);

    while(true) {
        uint16_t adc_val = adc_read();
        float temperature_celsius = adc_to_celsius(adc_val);
        float temperature_fahrenheit = celsius_to_fahrenheit(temperature_celsius);

        char temp_celsius_str[17] = "               ";
        get_c_string_from_float(temperature_celsius, temp_celsius_str);
        center_c_str(temp_celsius_str);

        char temp_fahrenheit_str[17] = "               ";
        get_c_string_from_float(temperature_fahrenheit, temp_fahrenheit_str);
        center_c_str(temp_fahrenheit_str);

        const unsigned int n_lines = 5;
        char* o_led_text[] = {
            "  TEMP CELSIUS  ",
            temp_celsius_str,
            "                ",
            "TEMP FAHRENHEIT ",
            temp_fahrenheit_str
        };
        clear_OLed();
        print_lines_OLed(o_led_text, n_lines, 0, 0);
        render_on_display(ssd, &frame_area);
        sleep_ms(UPDATE_PERIOD);
    }

    return 0;
}

void center_c_str(char* str) {
    const int white_spaces = (MAX_CHAR - strlen(str)) >> 1;
    if (white_spaces <= 0) {
        return;
    }
    for (int i = strlen(str); i >= 0; i--) {
        str[i + white_spaces] = str[i];
        str[i] = ' ';
    }
}

void get_c_string_from_int(int n, char* str) {
    snprintf(str, sizeof(str), "%d", n);
}

void get_c_string_from_float(float n, char *str) {
    snprintf(str, sizeof(str), "%.2f", n);
}
