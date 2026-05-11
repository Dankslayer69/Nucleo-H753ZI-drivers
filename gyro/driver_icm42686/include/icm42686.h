#ifndef ICM42686_H
#define ICM42686_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "icm42686_params.h"
#include "icm42686_regs.h"

typedef enum {
    ICM42686_OK = 0,
    ICM42686_ERROR = -1,
    ICM42686_INVALID_PARAM = -2,
    ICM42686_DEVICE_MISMATCH = -3,
} icm42686_status_t;

typedef int32_t (*icm42686_write_fn)(void *context, uint8_t reg, const uint8_t *data, uint16_t length);
typedef int32_t (*icm42686_read_fn)(void *context, uint8_t reg, uint8_t *data, uint16_t length);

typedef struct {
    void *context;
    icm42686_write_fn write;
    icm42686_read_fn read;
} icm42686_bus_t;

typedef struct {
    icm42686_bus_t bus;
} icm42686_dev_t;

typedef struct {
    int16_t x;
    int16_t y;
    int16_t z;
} icm42686_raw_axis_t;

icm42686_status_t icm42686_init(icm42686_dev_t *dev);
icm42686_status_t icm42686_get_who_am_i(icm42686_dev_t *dev, uint8_t *whoami);
icm42686_status_t icm42686_read_accel_raw(icm42686_dev_t *dev, icm42686_raw_axis_t *accel);
icm42686_status_t icm42686_read_gyro_raw(icm42686_dev_t *dev, icm42686_raw_axis_t *gyro);

icm42686_status_t icm42686_set_gyro_config(icm42686_dev_t *dev, uint8_t config_byte);
icm42686_status_t icm42686_set_accel_config(icm42686_dev_t *dev, uint8_t config_byte);

#ifdef __cplusplus
}
#endif

#endif // ICM42686_H
