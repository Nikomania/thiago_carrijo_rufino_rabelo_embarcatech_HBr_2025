/* Example code to talk to a MPU6050 MEMS accelerometer and gyroscope

  This is taking to simple approach of simply reading registers. It's perfectly
  possible to link up an interrupt line and set things up to read from the
  inbuilt FIFO to make it more useful.

  NOTE: Ensure the device is capable of being driven at 3.3v NOT 5v. The Pico
  GPIO (and therefore I2C) cannot be used at 5v.

  You will need to use a level shifter on the I2C lines if you want to run the
  board at 5v.

  Connections on Raspberry Pi Pico board, other boards may vary.

  GPIO I2C_SDA_PIN (On Pico this is GP4 (pin 6)) -> SDA on MPU6050 board
  GPIO I2C_SCL_PIN (On Pico this is GP5 (pin 7)) -> SCL on MPU6050 board
  3.3v (pin 36) -> VCC on MPU6050 board
  GND (pin 38)  -> GND on MPU6050 board
*/

// By default these devices  are on bus address 0x68
#ifndef MPU6050_H
#define MPU6050_H
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"


#define I2C_SDA_PIN 0
#define I2C_SCL_PIN 1

#define MPU6050_ADDR        0x68
#define WHO_AM_I_REG        0x75
#define PWR_MGMT_1_REG      0x6B
#define SMPLRT_DIV_REG      0x19
#define CONFIG_REG          0x1A
#define GYRO_CONFIG_REG     0x1B
#define ACCEL_CONFIG_REG    0x1C
#define ACCEL_XOUT_H        0x3B

#define GRAVITY 9.80665f // m/s²
#define COEF_LSB2MS2 (2 * GRAVITY / (1 << 15)) // ±2g → 16384 LSB/g


#ifdef i2c_default
bool mpu6050_init();
void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t accel_offset[3], int16_t gyro_offset[3], int16_t *temp);
void mpu6050_byte_to_m_per_s_squared(int16_t accel[3], float accel_g[3]);
void mpu6050_byte_to_dps(int16_t gyro[3], float gyro_dps[3]);
void mpu6050_byte_to_celsius(int16_t temp_buf, float* temp);
// void mpu6050_calibrate(int16_t accel_offset[3], int16_t gyro_offset[3], int16_t accel_scale[3]);
#endif

#endif
