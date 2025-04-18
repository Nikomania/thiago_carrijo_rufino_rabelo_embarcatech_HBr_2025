#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

#define ADC_TEMPERATURE_CHANNEL 4

#define MAX_LINES 8
#define CHAR_HEIGHT 8
#define MAX_CHAR 16
#define I2C_SDA 14
#define I2C_SCL 15

#define UPDATE_PERIOD 1000 // ms

// ssd1306_width pixels por ssd1306_n_pages páginas
struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};

uint8_t ssd[ssd1306_buffer_length];

void init_OLed();

void center_c_str(char *str);
void get_c_string_from_int(int n, char *str);
void get_c_string_from_float(float n, char *str);

void print_lines_OLed(char **text, unsigned int n_lines, int x0, int y0);
void clear_OLed();

float adc_to_temperature(uint16_t adc_value);
float celsius_to_fahrenheit(float temperature_celsius);

int main() {
    stdio_init_all();

    init_OLed();
    clear_OLed();

    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(ADC_TEMPERATURE_CHANNEL);

    while(true) {
        uint16_t adc_value = adc_read();
        float temperature_celsius = adc_to_temperature(adc_value);
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

float adc_to_temperature(uint16_t adc_value) {
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = adc_value * conversion_factor;
    float temperature = 27.0f - (voltage - 0.706f) / 0.001721f;
    return temperature;
}

float celsius_to_fahrenheit(float temperature_celsius) {
    return 1.8f * temperature_celsius + 32;
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

void print_lines_OLed(char** text, unsigned int n_lines, int x0, int y0) {
    if (n_lines > MAX_LINES || y0 + n_lines * 8 > ssd1306_height) {
        printf("OLed lines exceeded (%d)\n", MAX_LINES);
        return;
    }
    int y = y0;
    for (uint i = 0; i < n_lines; i++) {
        ssd1306_draw_string(ssd, x0, y, text[i]);
        y += CHAR_HEIGHT;
    }
}

void clear_OLed() {
    memset(ssd, 0, ssd1306_buffer_length);
}

void init_OLed() {
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // init do OLED SSD1306
    ssd1306_init();

    calculate_render_area_buffer_length(&frame_area);
}

// AFTER INIT OLED
// Parte do código para exibir a mensagem no display (opcional: mudar ssd1306_height para 32 em ssd1306_i2c.h)
    // /**

    // char value[17];
    // get_c_string_from_int(12, value);

    // const unsigned int n_lines = 2;
    // char* text[] = {
    //     value,
    //     "A Embarcatech BA"
    // };

    // print_lines_OLed(text, n_lines, 0, 0);
    // */

    // Parte do código para exibir a linha no display (algoritmo de Bresenham)
    /**
        ssd1306_draw_line(ssd, 10, 10, 100, 50, true);
        render_on_display(ssd, &frame_area);
    */

    // Parte do código para exibir o bitmap no display
    /**
        const uint8_t bitmap_128x64[] = { 
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0xaa,0xaa,0xaa,0xaa,0xaa,0x0a,0x00,0x00,0x10,
    0x11,0x11,0x11,0x11,0x11,0x00,0x00,0x4a,0x4a,0x4a,0x4a,0x4a,0x0a,0x00,0x00,
    0x20,0x21,0x21,0x21,0x21,0x01,0x00,0x00,0x4a,0x94,0x94,0x94,0x94,0x04,0x00,
    0x00,0x10,0x21,0x22,0x22,0x22,0x12,0x00,0x00,0x4a,0x4a,0x44,0x44,0x44,0x04,
    0x00,0x00,0x90,0x90,0x92,0x92,0x92,0x02,0x00,0x00,0x22,0x25,0x24,0x24,0x24,
    0x14,0x00,0x00,0x14,0x92,0x92,0x92,0x92,0x02,0x00,0x00,0x00,0x00,0x48,0x04,
    0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
    0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
    0x50,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x84,0x04,0x00,0x00,0x00,0x00,0x00,
    0x00,0x28,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x94,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x04,0x00,0x00,0x00,
    0x00,0x00,0x00,0x48,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0a,0x00,0x00,
    0x00,0x00,0x00,0x00,0x40,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x02,0x00,
    0x00,0x00,0x00,0x00,0x00,0xa4,0x04,0x00,0x00,0x00,0x00,0x20,0x92,0x48,0x92,
    0x24,0x09,0x00,0x00,0x4a,0x49,0x92,0x48,0x92,0x04,0x00,0x00,0x10,0x84,0x44,
    0x22,0x44,0x10,0x00,0x00,0x4a,0x51,0x12,0x89,0x12,0x05,0x00,0x00,0x10,0x8a,
    0x48,0x52,0xa4,0x08,0x00,0x00,0x4a,0x11,0x91,0x88,0x08,0x05,0x00,0x00,0x20,
    0xa2,0x24,0x22,0xa5,0x10,0x00,0x00,0x4a,0x49,0x92,0x94,0x10,0x0a,0x00,0x00,
    0x10,0x12,0x49,0x22,0x4a,0x01,0x00,0x00,0xa0,0x24,0x22,0x89,0x10,0x02,0x00,
    0x00,0x10,0x91,0x88,0x50,0x4a,0x01,0x00,0x00,0x40,0x24,0x25,0x8a,0x24,0x00,
    0x00,0x00,0x80,0x92,0x48,0x11,0x91,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x01,0x24,0x04,0x00,0x00,0x00,0x00,
    0x4a,0x02,0x88,0x02,0x00,0x00,0x00,0x00,0x90,0x00,0x24,0x09,0x00,0x00,0x00,
    0x00,0x24,0x02,0x48,0x04,0x00,0x00,0x00,0x00,0x88,0x04,0x22,0x01,0x00,0x00,
    0x00,0x00,0x52,0x02,0x88,0x14,0x00,0x00,0x00,0x00,0x84,0x00,0x24,0x01,0x00,
    0x00,0x00,0x00,0x50,0x02,0x48,0x0a,0x00,0x00,0x00,0x00,0x24,0x04,0x24,0x00,
    0x00,0x00,0x00,0x00,0x84,0x02,0x90,0x0a,0x00,0x00,0x00,0x00,0x28,0x01,0x44,
    0x02,0x10,0x00,0x00,0x00,0x44,0x00,0x28,0x09,0x00,0x00,0x00,0x01,0x90,0x00,
    0x40,0x04,0x20,0x00,0x80,0x00,0x48,0x00,0x14,0x01,0x40,0x00,0x00,0x00,0x00,
    0x00,0xa0,0x00,0x90,0x00,0x40,0x01,0x00,0x00,0x00,0x00,0x20,0x00,0x20,0x02,
    0x00,0x00,0x00,0x00,0x40,0x01,0x40,0x01,0x00,0x00,0x00,0x00,0x10,0x02,0x28,
    0x00,0x00,0x00,0x00,0x00,0xa0,0x00,0x40,0x05,0x00,0x00,0x00,0x00,0x08,0x05,
    0x10,0x02,0x00,0x00,0x00,0x00,0x50,0x02,0xa0,0x08,0x00,0x20,0x00,0x00,0x24,
    0x01,0x10,0x25,0x00,0x00,0x00,0x00,0x92,0x00,0x40,0x48,0x00,0xa8,0x00,0x00,
    0x09,0x00,0x80,0x92,0x02,0x45,0x00,0x40,0xa4,0x00,0x40,0x24,0x54,0x28,0x05,
    0x00,0x11,0x00,0x00,0x89,0x22,0x05,0x00,0x00,0x44,0x00,0x00,0x22,0x94,0x08,
    0x00,0x00,0x28,0x00,0x00,0x49,0x21,0x01,0x00,0x00,0x00,0x00,0x00,0x24,0x8a,
    0x04,0x00,0x00,0x14,0x00,0x00,0x88,0x24,0x00,0x00,0x00,0x08,0x00,0x00,0x24,
    0x91,0x00,0x00,0x00,0x04,0x00,0x00,0x90,0x24,0x01,0x00,0x00,0x00,0x00,0x00,
    0x20,0x49,0x00,0x00,0x00,0x04,0x00,0x00,0x10,0x22,0x80,0xaa,0xaa,0x02,0x00,
    0x00,0x40,0x89,0x00,0x00,0x40,0x00,0x00,0x00,0x80,0x24,0x80,0xaa,0x2a,0x01,
    0x00,0x00,0x40,0x48,0x00,0x92,0x44,0x00,0x00,0x00,0x00,0x25,0x80,0x24,0x09,
    0x00,0x00,0x00,0x00,0x91,0x00,0x11,0x52,0x00,0x00,0x00,0x00,0x44,0x00,0xa2,
    0x08,0x00,0x00,0x00,0x00,0x29,0x80,0x14,0x05,0x00,0x00,0x00,0x00,0x84,0x00,
    0x21,0x10,0x00,0x00,0x00,0x00,0x50,0x00,0x4a,0x05,0x00,0x00,0x00,0x00,0x08,
    0x80,0x10,0x01,0x00,0x00,0x00,0x00,0x50,0x11,0xa5,0x04,0x00,0x00,0x00,0x00,
    0x80,0xa4,0x08,0x02,0x00,0x00,0x00,0x00,0x50,0x12,0x52,0x01,0x00,0x00,0x00,
    0x00,0x00,0x49,0x09,0x00,0x00,0x00,0x00,0x00,0x40,0x84,0xa4,0x00,0x00,0x00,
    0x00,0x00,0x80,0x52,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x25,0x00,0x00,
    0x00,0x00,0x00,0x00,0xa5,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x11,0x00,
    0x00,0x00,0x00,0x00,0x00,0x52,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x84,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x50,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x10,
    0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xa0,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00 };

        ssd1306_t ssd_bm;
        ssd1306_init_bm(&ssd_bm, 128, 64, false, 0x3C, i2c1);
        ssd1306_config(&ssd_bm);

        ssd1306_draw_bitmap(&ssd_bm, bitmap_128x64);
    */
