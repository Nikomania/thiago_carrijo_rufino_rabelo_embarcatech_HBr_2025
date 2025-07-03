#include "MPU6050.h"

int addr = 0x68;

#ifdef i2c_default
void mpu6050_reset() {
    // Two byte reset. First byte register, second byte data
    // There are a load more options to set up the device in different ways that could be added here
    uint8_t buf[] = {0x6B, 0x80};
    i2c_write_blocking(i2c_default, addr, buf, 2, false);
    sleep_ms(100); // Allow device to reset and stabilize

    // Clear sleep mode (0x6B register, 0x00 value)
    buf[1] = 0x00;  // Clear sleep mode by writing 0x00 to the 0x6B register
    i2c_write_blocking(i2c_default, addr, buf, 2, false); 
    sleep_ms(10); // Allow stabilization after waking up
}

void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t accel_offset[3], int16_t gyro_offset[3], int16_t *temp) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    uint8_t buffer[6];

    // Start reading acceleration registers from register 0x3B for 6 bytes
    uint8_t val = 0x3B;
    i2c_write_blocking(i2c_default, addr, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(i2c_default, addr, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]) - accel_offset[i];
    }

    // Now gyro data from reg 0x43 for 6 bytes
    // The register is auto incrementing on each read
    val = 0x43;
    i2c_write_blocking(i2c_default, addr, &val, 1, true);
    i2c_read_blocking(i2c_default, addr, buffer, 6, false);  // False - finished with bus

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]) - gyro_offset[i];
    }

    // Now temperature from reg 0x41 for 2 bytes
    // The register is auto incrementing on each read
    val = 0x41;
    i2c_write_blocking(i2c_default, addr, &val, 1, true);
    i2c_read_blocking(i2c_default, addr, buffer, 2, false);  // False - finished with bus

    *temp = buffer[0] << 8 | buffer[1];
}

void mpu6050_byte_to_acceleration(int16_t accel[3], float accel_g[3]) {
    // The MPU6050 has a range of +/- 2g, so the raw values need to be divided by 16384.0
    // to get the acceleration in g's.
    for (int i = 0; i < 3; i++) {
        accel_g[i] = accel[i] * 9.8 / (2 << 13); // 2^13 = 8192, so 2g = 8192
    }
}

void mpu6050_byte_to_gyro(int16_t gyro[3], float gyro_dps[3]) {
    // The MPU6050 has a range of +/- 250 degrees per second, so the raw values need to be divided by 131.0
    // to get the angular velocity in degrees per second.
    for (int i = 0; i < 3; i++) {
        gyro_dps[i] = gyro[i] / 131.0; // 250 dps = 131 LSB/dps
    }
}

void mpu6050_calibrate(int16_t accel_offset[3], int16_t gyro_offset[3]) {
    // Simple calibration routine that averages the values over a period of time
    // to find the offsets. This is a basic approach and may not be suitable for all applications.
    int16_t accel_buf[3] = {0}, gyro_buf[3] = {0};
    int32_t accel_acc[3] = {0}, gyro_acc[3] = {0};
    const int samples = 16; // Number of samples to average
    int16_t temp;

    for (int i = 0; i < samples; i++) {
        mpu6050_read_raw(accel_buf, gyro_buf, accel_offset, gyro_offset, &temp);
        for (int j = 0; j < 3; j++) {
            accel_acc[j] += accel_buf[j];
            gyro_acc[j] += gyro_buf[j];
        }
        sleep_ms(100); // Small delay to avoid flooding the I2C bus
    }

    for (int i = 0; i < 3; i++) {
        accel_offset[i] = accel_acc[i] / samples;
        gyro_offset[i] = gyro_acc[i] / samples;
    }
}
#endif
