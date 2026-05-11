#ifndef ICM42686_CMSIS_H
#define ICM42686_CMSIS_H

#include "icm42686.h"
#include "stm32h7xx.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    I2C_TypeDef *i2c;
    uint8_t dev_addr;
} icm42686_cmsis_i2c_context_t;

icm42686_status_t icm42686_cmsis_i2c_init(icm42686_dev_t *dev,
                                          I2C_TypeDef *i2c,
                                          uint8_t dev_addr);

#ifdef __cplusplus
}
#endif

#endif // ICM42686_CMSIS_H
