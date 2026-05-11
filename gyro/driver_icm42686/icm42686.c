#include "include/icm42686.h"
#include <stddef.h>

static inline uint16_t icm42686_combine16(uint8_t msb, uint8_t lsb)
{
    return (uint16_t)((msb << 8) | lsb);
}

static icm42686_status_t icm42686_bus_write(icm42686_dev_t *dev, uint8_t reg, const uint8_t *data, uint16_t length)
{
    if (!dev || !dev->bus.write) {
        return ICM42686_INVALID_PARAM;
    }
    return dev->bus.write(dev->bus.context, reg, data, length) == 0 ? ICM42686_OK : ICM42686_ERROR;
}

static icm42686_status_t icm42686_bus_read(icm42686_dev_t *dev, uint8_t reg, uint8_t *data, uint16_t length)
{
    if (!dev || !dev->bus.read || !data) {
        return ICM42686_INVALID_PARAM;
    }
    return dev->bus.read(dev->bus.context, reg, data, length) == 0 ? ICM42686_OK : ICM42686_ERROR;
}

icm42686_status_t icm42686_get_who_am_i(icm42686_dev_t *dev, uint8_t *whoami)
{
    if (!whoami) {
        return ICM42686_INVALID_PARAM;
    }

    uint8_t value = 0;
    icm42686_status_t status = icm42686_bus_read(dev, ICM42686_WHO_AM_I, &value, 1);
    if (status != ICM42686_OK) {
        return status;
    }

    *whoami = value;
    return (value == ICM42686_WHO_AM_I_EXPECTED) ? ICM42686_OK : ICM42686_DEVICE_MISMATCH;
}

icm42686_status_t icm42686_init(icm42686_dev_t *dev)
{
    if (!dev) {
        return ICM42686_INVALID_PARAM;
    }

    uint8_t whoami = 0;
    icm42686_status_t status = icm42686_get_who_am_i(dev, &whoami);
    if (status != ICM42686_OK) {
        return status;
    }

    return ICM42686_OK;
}

icm42686_status_t icm42686_set_gyro_config(icm42686_dev_t *dev, uint8_t config_byte)
{
    return icm42686_bus_write(dev, ICM42686_GYRO_CONFIG0, &config_byte, 1);
}

icm42686_status_t icm42686_set_accel_config(icm42686_dev_t *dev, uint8_t config_byte)
{
    return icm42686_bus_write(dev, ICM42686_ACCEL_CONFIG0, &config_byte, 1);
}

icm42686_status_t icm42686_read_accel_raw(icm42686_dev_t *dev, icm42686_raw_axis_t *accel)
{
    if (!accel) {
        return ICM42686_INVALID_PARAM;
    }

    uint8_t buffer[6] = {0};
    icm42686_status_t status = icm42686_bus_read(dev, ICM42686_ACCEL_DATA_START, buffer, sizeof(buffer));
    if (status != ICM42686_OK) {
        return status;
    }

    accel->x = (int16_t)icm42686_combine16(buffer[0], buffer[1]);
    accel->y = (int16_t)icm42686_combine16(buffer[2], buffer[3]);
    accel->z = (int16_t)icm42686_combine16(buffer[4], buffer[5]);
    return ICM42686_OK;
}

icm42686_status_t icm42686_read_gyro_raw(icm42686_dev_t *dev, icm42686_raw_axis_t *gyro)
{
    if (!gyro) {
        return ICM42686_INVALID_PARAM;
    }

    uint8_t buffer[6] = {0};
    icm42686_status_t status = icm42686_bus_read(dev, ICM42686_GYRO_DATA_START, buffer, sizeof(buffer));
    if (status != ICM42686_OK) {
        return status;
    }

    gyro->x = (int16_t)icm42686_combine16(buffer[0], buffer[1]);
    gyro->y = (int16_t)icm42686_combine16(buffer[2], buffer[3]);
    gyro->z = (int16_t)icm42686_combine16(buffer[4], buffer[5]);
    return ICM42686_OK;
}
