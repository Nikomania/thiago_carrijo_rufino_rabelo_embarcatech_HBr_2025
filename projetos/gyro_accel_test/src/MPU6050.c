#include "MPU6050.h"
#include "gpio.h"


#ifdef i2c_default
bool mpu6050_init() {
    uint8_t buf[2];

    buf[0] = PWR_MGMT_1_REG; buf[1] = 0x00;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, buf, 2, false);
    sleep_ms(100);

    buf[0] = 0x1C; // ACCEL_CONFIG_REG
    buf[1] = 0x00; // full-scale range ±2g
    i2c_write_blocking(i2c_default, MPU6050_ADDR, buf, 2, false);
    sleep_ms(10);

    buf[0] = 0x1B; // GYRO_CONFIG_REG
    buf[1] = 0x00; // full-scale range ±250dps
    i2c_write_blocking(i2c_default, MPU6050_ADDR, buf, 2, false);
    sleep_ms(10);

    // 3) verificar WHO_AM_I
    uint8_t who = 0;
    buf[0] = WHO_AM_I_REG;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, buf, 1, true);
    i2c_read_blocking (i2c_default, MPU6050_ADDR, &who,    1, false);
    if (who != MPU6050_ADDR && who != MPU6050_ADDR + 8) {
        // erro de comunicação
        printf("MPU6050: WHO_AM_I = 0x%02X (esperado 0x%02X)\n", who, MPU6050_ADDR);
        return false;
    }

    return true;
}

void mpu6050_read_accel(int16_t accel_raw[3]) {
    uint8_t reg = ACCEL_XOUT_H;
    uint8_t buf[6];
    i2c_write_blocking(i2c_default, MPU6050_ADDR, &reg, 1, true);
    i2c_read_blocking(i2c_default, MPU6050_ADDR, buf, 6, false);
    for (int i = 0; i < 3; i++) {
        accel_raw[i] = (buf[2*i] << 8) | buf[2*i+1];
    }
}

void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t accel_offset[3], int16_t gyro_offset[3], int16_t *temp) {
    // For this particular device, we send the device the register we want to read
    // first, then subsequently read from the device. The register is auto incrementing
    // so we don't need to keep sending the register we want, just the first.

    uint8_t buffer[6];

    // Start reading acceleration registers from register 0x3B for 6 bytes
    uint8_t val = 0x3B;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, &val, 1, true); // true to keep master control of bus
    i2c_read_blocking(i2c_default, MPU6050_ADDR, buffer, 6, false);

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]) - accel_offset[i];
    }

    // Now gyro data from reg 0x43 for 6 bytes
    // The register is auto incrementing on each read
    val = 0x43;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, MPU6050_ADDR, buffer, 6, false);  // False - finished with bus

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8 | buffer[(i * 2) + 1]) - gyro_offset[i];
    }

    // Now temperature from reg 0x41 for 2 bytes
    // The register is auto incrementing on each read
    val = 0x41;
    i2c_write_blocking(i2c_default, MPU6050_ADDR, &val, 1, true);
    i2c_read_blocking(i2c_default, MPU6050_ADDR, buffer, 2, false);  // False - finished with bus

    *temp = buffer[0] << 8 | buffer[1];
}

void mpu6050_byte_to_m_per_s_squared(int16_t accel_raw[3], float accel_m_s2[3]) {
    // ±2g → 16384 LSB/g
    for (int i = 0; i < 3; i++) {
        accel_m_s2[i] = accel_raw[i] * COEF_LSB2MS2;
    }
}

void mpu6050_byte_to_dps(int16_t gyro_raw[3], float gyro_dps[3]) {
    // ±250°/s → 131 LSB/°/s
    for (int i = 0; i < 3; i++) {
        gyro_dps[i] = gyro_raw[i] / 131.0f;
    }
}

void mpu6050_byte_to_celsius(int16_t temp_raw, float *t_celsius) {
    // sensoriamento de temperatura
    *t_celsius = (temp_raw / 340.0f) + 36.53f;
}

// void mpu6050_calibrate(int16_t accel_offset[3], int16_t gyro_offset[3], int16_t accel_scale[3]) {
//     const char* prompts[6] = {
//         ">> Posicione +Z para cima e aperte A",
//         ">> Posicione -Z para cima e aperte A",
//         ">> Posicione +Y para cima e aperte A",
//         ">> Posicione -Y para cima e aperte A",
//         ">> Posicione +X para cima e aperte A",
//         ">> Posicione -X para cima e aperte A"
//     };
    
//     float R[3][2];

//     printf("=== Calibracao Automatica MPU6050 ===\n");
//     printf("Pressione A para iniciar...\n");

//     // Espera primeiro aperto
//     while (!is_button_pressed());
//     sleep_ms(50);
//     while (is_button_pressed());
//     sleep_ms(50);

//     int state = 0;

//     while (state < 6) {
//         printf("%s\n", prompts[state]);
//         // aguarda apertar
//         while (!is_button_pressed());
//         sleep_ms(50);
//         while (is_button_pressed());
//         sleep_ms(50);

//         // lê accel raw e converte
//         int16_t raw[3];
//         mpu6050_read_accel(raw);
//         float v = raw[state/2] * COEF_LSB2MS2;
//         R[state/2][state%2] = v;
//         printf("Leitura eixo %d %s: %f m/s²\n", state/2,
//                (state%2==0?"+1g":"-1g"), v);
//         state++;
//     }

//     // calcula offset e scale
//     for (int axe = 0; axe < 3; axe++) {
//         accel_offset[axe] = 0.5f * (R[axe][0] + R[axe][1]);
//         accel_scale[axe]  = 0.5f * (R[axe][0] - R[axe][1]) / GRAVITY;
//     }

//     // exibe resultados
//     printf("\n=== Resultado da Calibracao ===\n");
//     for (int axe = 0; axe < 3; axe++) {
//         const char* name = axe==0?"X":axe==1?"Y":"Z";
//         printf("Offset %s: %f m/s², Scale %s: %f\n", name, accel_offset[axe], name, accel_scale[axe]);
//     }

//     printf("Calibracao concluida.\n");
// }
#endif
