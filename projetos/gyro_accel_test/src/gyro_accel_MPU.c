/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "pico/stdlib.h"
#include "MPU6050.h"

#define DELAY 100 // ms
#define ALPHA 0.9f

float calc_angle(float accel[3], float gyro[3], float alpha, float dt) {
    static float angle = 0.0f; // Initialize angle to zero
    float accel_angle = atan2f(accel[2], accel[0]) * 180.0f / M_PI;
    angle = alpha * (angle + gyro[1] * dt) + (1 - alpha) * accel_angle;
    return angle;
}

int main() {
    stdio_init_all();
    sleep_ms(5000); // Wait for the serial port to be ready
#if !defined(i2c_default) || !defined(I2C_SDA_PIN) || !defined(I2C_SCL_PIN)
    #warning i2c/mpu6050_i2c example requires a board with I2C pins
    puts("Default I2C pins were not defined");
    return 0;
#else
    printf("Hello, MPU6050! Reading raw data from registers...\n");

    // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(I2C_SDA_PIN, I2C_SCL_PIN, GPIO_FUNC_I2C));

    if (!mpu6050_init()) {
        while (true) {
            printf("MPU6050 initialization failed. Please check your connections.\n");
            sleep_ms(1000); // Wait before retrying
        }
    } else {
        printf("MPU6050 initialized successfully.\n");
    }
    
    int16_t accel_buf[3], gyro_buf[3], accel_offset[3] = {0}, gyro_offset[3] = {0}, accel_scale[3], temp_buf;
    float accel_g[3], gyro_dps[3], temp;

    // mpu6050_calibrate(accel_offset, gyro_offset, accel_scale);

    printf("Starting to read raw data from MPU6050...\n");

    while (1) {
        mpu6050_read_raw(accel_buf, gyro_buf, accel_offset, gyro_offset, &temp_buf);
        mpu6050_byte_to_m_per_s_squared(accel_buf, accel_g);
        mpu6050_byte_to_dps(gyro_buf, gyro_dps);
        mpu6050_byte_to_celsius(temp_buf, &temp);
        float angle = calc_angle(accel_g, gyro_dps, ALPHA, DELAY / 1000.0f);

        // These are the raw numbers from the chip, so will need tweaking to be really useful.
        // See the datasheet for more information
        printf("Acc. X = %.2f, Y = %.2f, Z = %.2f\n", accel_g[0], accel_g[1], accel_g[2]);
        printf("Gyro. X = %.2f, Y = %.2f, Z = %.2f\n", gyro_dps[0], gyro_dps[1], gyro_dps[2]);
        printf("Temp. = %.2f\n", temp);
        printf("Angle = %.2f\n", angle);

        sleep_ms(DELAY);
    }
#endif
}
