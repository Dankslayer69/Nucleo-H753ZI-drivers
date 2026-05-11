#include "stm32h7xx_hal.h"
#include "driver_icm42686/include/icm42686.h"
#include "driver_icm42686/include/icm42686_cmsis.h"

int main(void)
{
    HAL_Init();

    icm42686_dev_t imu;
    uint8_t sensor_address = ICM42686_I2C_ADDRESS_LOW;

    if (icm42686_cmsis_i2c_init(&imu, I2C1, sensor_address) != ICM42686_OK) {
        Error_Handler();
    }

    if (icm42686_init(&imu) != ICM42686_OK) {
        Error_Handler();
    }

    uint8_t gyro_config = 0x00; // configure full-scale and ODR as needed
    uint8_t accel_config = 0x00; // configure full-scale and ODR as needed

    icm42686_set_gyro_config(&imu, gyro_config);
    icm42686_set_accel_config(&imu, accel_config);

    icm42686_raw_axis_t gyro;
    icm42686_raw_axis_t accel;

    while (1) {
        if (icm42686_read_gyro_raw(&imu, &gyro) == ICM42686_OK &&
            icm42686_read_accel_raw(&imu, &accel) == ICM42686_OK) {
            // use gyro.x, gyro.y, gyro.z and accel.x, accel.y, accel.z
        }

        HAL_Delay(10);
    }
}
