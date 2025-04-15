#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"

#define DEBOUNCE 50
#define ADC_BITS 12
#define ADC_MAX (1 << ADC_BITS) - 1 // 12-bit ADC
#define JST_X_PIN 27
#define JST_Y_PIN 26

#define MAX_LINES 8
#define CHAR_HEIGHT 8
#define MAX_CHAR 16
#define I2C_SDA 14
#define I2C_SCL 15

#define FPS 120
#define FPS_PERIOD (1000 / FPS) // ms

// ssd1306_width pixels por ssd1306_n_pages páginas
struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};

uint8_t ssd[ssd1306_buffer_length];

int jst_x_percentage;
int jst_y_percentage;

void init_OLed();
void init_gpio();

void center_c_str(char *str);
void get_c_string_from_int(int n, char *str);

void print_lines_OLed(char **text, unsigned int n_lines, int x0, int y0);
void clear_OLed();

void update_joystick();

int main() {
    stdio_init_all();

    init_OLed();
    clear_OLed();

    init_gpio();

    while(true) {
        update_joystick();

        char jst_x_str[17] = "               ";
        get_c_string_from_int(jst_x_percentage, jst_x_str);
        center_c_str(jst_x_str);

        char jst_y_str[17] = "               ";
        get_c_string_from_int(jst_y_percentage, jst_y_str);
        center_c_str(jst_y_str);

        const unsigned int n_lines = 5;
        char* o_led_text[] = {
            " JST X PERCENT  ",
            jst_x_str,
            "                ",
            " JST Y PERCENT  ",
            jst_y_str
        };
        clear_OLed();
        print_lines_OLed(o_led_text, n_lines, 0, 0);
        render_on_display(ssd, &frame_area);
        sleep_ms(FPS_PERIOD);
    }

    return 0;
}

void update_joystick() {
    uint16_t jst_adc_raw;

    adc_select_input(0);
    jst_adc_raw = adc_read();
    jst_x_percentage = (jst_adc_raw * 100) / ADC_MAX; // 0-100%

    adc_select_input(1);
    jst_adc_raw = adc_read();
    jst_y_percentage = (jst_adc_raw * 100) / ADC_MAX; // 0-100%
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

void init_gpio() {
    adc_init();
    // Make sure GPIO is high-impedance, no pullups etc
    adc_gpio_init(JST_Y_PIN);
    adc_gpio_init(JST_X_PIN);
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
