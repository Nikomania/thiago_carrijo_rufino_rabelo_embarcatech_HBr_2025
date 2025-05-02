#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <stdint.h>

float adc_to_celsius(uint16_t adc_val);
float celsius_to_fahrenheit(float temperature_celsius);

#endif // TEMPERATURE_H
