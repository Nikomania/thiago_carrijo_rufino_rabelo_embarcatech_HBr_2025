/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "MPU6050.h"


int main() {
    stdio_init_all();
    sleep_ms(3000); // Wait for the serial port to be ready
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

    mpu6050_reset();
    
    int16_t accel_buf[3], gyro_buf[3], accel_offset[3], gyro_offset[3], temp;
    float accel_g[3], gyro_dps[3];

    printf("MPU6050 will calibrate offsets for 2 seconds in 3 seconds...\n");

    printf("Please keep the device still during calibration.\n");
    sleep_ms(3000);

    mpu6050_calibrate(accel_offset, gyro_offset);

    printf("Calibration complete.\n");
    
    while (1) {
        mpu6050_read_raw(accel_buf, gyro_buf, accel_offset, gyro_offset, &temp);
        mpu6050_byte_to_acceleration(accel_buf, accel_g);
        mpu6050_byte_to_gyro(gyro_buf, gyro_dps);

        // These are the raw numbers from the chip, so will need tweaking to be really useful.
        // See the datasheet for more information
        printf("Acc. X = %.2f, Y = %.2f, Z = %.2f\n", accel_g[0], accel_g[1], accel_g[2]);
        printf("Gyro. X = %.2f, Y = %.2f, Z = %.2f\n", gyro_dps[0], gyro_dps[1], gyro_dps[2]);
        // Temperature is simple so use the datasheet calculation to get deg C.
        // Note this is chip temperature.
        printf("Temp. = %f\n", (temp / 340.0) + 36.53);

        sleep_ms(100);
    }
#endif
}
