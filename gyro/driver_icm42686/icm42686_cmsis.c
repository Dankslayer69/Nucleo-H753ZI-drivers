#include "include/icm42686_cmsis.h"

#define ICM42686_I2C_TIMEOUT 100000u

static int32_t icm42686_cmsis_wait_flag(I2C_TypeDef *i2c, uint32_t flag)
{
    uint32_t timeout = ICM42686_I2C_TIMEOUT;
    while (!(i2c->ISR & flag)) {
        if (i2c->ISR & I2C_ISR_NACKF) {
            i2c->ICR = I2C_ICR_NACKCF;
            return -1;
        }
        if (!timeout--) {
            return -1;
        }
    }
    return 0;
}

static int32_t icm42686_cmsis_write(void *context, uint8_t reg, const uint8_t *data, uint16_t length)
{
    if (!context || !data) {
        return -1;
    }

    icm42686_cmsis_i2c_context_t *ctx = (icm42686_cmsis_i2c_context_t *)context;
    I2C_TypeDef *i2c = ctx->i2c;
    uint32_t address = ((uint32_t)ctx->dev_addr << 1);
    uint32_t count = length + 1;

    i2c->CR2 = (address << I2C_CR2_SADD_Pos) |
               (count << I2C_CR2_NBYTES_Pos) |
               I2C_CR2_AUTOEND |
               I2C_CR2_START;

    if (icm42686_cmsis_wait_flag(i2c, I2C_ISR_TXIS) != 0) {
        return -1;
    }

    i2c->TXDR = reg;

    for (uint16_t i = 0; i < length; ++i) {
        if (icm42686_cmsis_wait_flag(i2c, I2C_ISR_TXIS) != 0) {
            return -1;
        }
        i2c->TXDR = data[i];
    }

    if (icm42686_cmsis_wait_flag(i2c, I2C_ISR_STOPF) != 0) {
        return -1;
    }

    i2c->ICR = I2C_ICR_STOPCF;
    return 0;
}

static int32_t icm42686_cmsis_read(void *context, uint8_t reg, uint8_t *data, uint16_t length)
{
    if (!context || !data || length == 0) {
        return -1;
    }

    icm42686_cmsis_i2c_context_t *ctx = (icm42686_cmsis_i2c_context_t *)context;
    I2C_TypeDef *i2c = ctx->i2c;
    uint32_t address = ((uint32_t)ctx->dev_addr << 1);

    i2c->CR2 = (address << I2C_CR2_SADD_Pos) |
               (1U << I2C_CR2_NBYTES_Pos) |
               I2C_CR2_RELOAD |
               I2C_CR2_START;

    if (icm42686_cmsis_wait_flag(i2c, I2C_ISR_TXIS) != 0) {
        return -1;
    }
    i2c->TXDR = reg;

    if (icm42686_cmsis_wait_flag(i2c, I2C_ISR_TC) != 0) {
        return -1;
    }

    i2c->CR2 = (address << I2C_CR2_SADD_Pos) |
               I2C_CR2_RD_WRN |
               (length << I2C_CR2_NBYTES_Pos) |
               I2C_CR2_AUTOEND |
               I2C_CR2_START;

    for (uint16_t i = 0; i < length; ++i) {
        if (icm42686_cmsis_wait_flag(i2c, I2C_ISR_RXNE) != 0) {
            return -1;
        }
        data[i] = (uint8_t)i2c->RXDR;
    }

    if (icm42686_cmsis_wait_flag(i2c, I2C_ISR_STOPF) != 0) {
        return -1;
    }

    i2c->ICR = I2C_ICR_STOPCF;
    return 0;
}

icm42686_status_t icm42686_cmsis_i2c_init(icm42686_dev_t *dev,
                                          I2C_TypeDef *i2c,
                                          uint8_t dev_addr)
{
    if (!dev || !i2c) {
        return ICM42686_INVALID_PARAM;
    }

    static icm42686_cmsis_i2c_context_t ctx;
    ctx.i2c = i2c;
    ctx.dev_addr = dev_addr;

    dev->bus.context = &ctx;
    dev->bus.write = icm42686_cmsis_write;
    dev->bus.read = icm42686_cmsis_read;

    return ICM42686_OK;
}
