#include "temperature.h"

float adc_to_celsius(uint16_t adc_val) {
    const float conversion_factor = 3.3f / (1 << 12);
    float voltage = adc_val * conversion_factor;
    return 27.0f - (voltage - 0.706f) / 0.001721f;
}

float celsius_to_fahrenheit(float temperature_celsius) {
    return 1.8f * temperature_celsius + 32;
}
