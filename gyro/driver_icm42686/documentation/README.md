# ICM-42686 Driver Technical Overview

## Project overview

This repository contains a lightweight driver implementation for the ICM-42686 6-axis IMU targeted at the Nucleo H753ZI platform. The driver is designed as a generic sensor layer combined with a CMSIS-based I2C transport adapter.

The key architectural goal is separation of concerns:

- sensor register semantics and data-format handling are implemented in `icm42686.c`
- platform-specific I2C transfer behavior is implemented in `icm42686_cmsis.c`
- public API definitions and device types are declared in `icm42686.h`

## Components

### `icm42686.h`

Defines:

- register addresses for ICM-42686 control and data registers
- device status codes (`ICM42686_OK`, `ICM42686_ERROR`, etc.)
- transport callback signatures
- the `icm42686_dev_t` device object
- raw axis data structure `icm42686_raw_axis_t`

This header is the public API boundary for the driver.

### `icm42686.c`

Implements the generic driver logic.

Responsibilities:

- device probing via `WHO_AM_I`
- register writes for `GYRO_CONFIG0` and `ACCEL_CONFIG0`
- raw data reads from `ACCEL_DATA_*` and `GYRO_DATA_*`
- converting 6-byte register bursts into signed 16-bit axis samples

The core driver is intentionally bus-agnostic and does not reference STM32 HAL or CMSIS I2C directly.

### `icm42686_cmsis.h`

Defines the CMSIS I2C transport adapter public interface.

It contains:

- the `icm42686_cmsis_i2c_context_t` transport context
- the initializer `icm42686_cmsis_i2c_init()`

### `icm42686_cmsis.c`

Implements the transport adapter for I2C using CMSIS `I2C_TypeDef` registers.

Key functions:

- `icm42686_cmsis_write()`
- `icm42686_cmsis_read()`

The CMSIS transport performs polling-mode I2C transactions using:

- `CR2` register programming for start, address, byte count, and auto-end
- `ISR` status polling for `TXIS`, `RXNE`, `TC`, `STOPF`, and `NACKF`
- `ICR` writes to clear interrupt flags

The adapter is responsible for converting the generic bus callback interface into concrete CMSIS register-level bus operations.

### `Makefile`

Builds a static library from `icm42686.c` and `icm42686_cmsis.c`.

This Makefile is intentionally minimal and assumes a host compiler. It is not a complete STM32 build system.

### `main.c`

Provides an integration example for the Nucleo platform.

It illustrates:

- MCU initialization via `HAL_Init()`
- binding the generic driver to `I2C1` via `icm42686_cmsis_i2c_init()`
- probing the sensor with `icm42686_init()`
- configuring the IMU with gyro/accel config writes
- executing periodic raw data acquisition

## Data flow

1. `main.c` initializes the board and constructs an `icm42686_dev_t`.
2. `icm42686_cmsis_i2c_init()` attaches the CMSIS transport to the device object.
3. `icm42686_init()` probes the sensor using `icm42686_get_who_am_i()`.
4. Configuration writes are issued to sensor registers.
5. Raw axis samples are read from consecutive data registers.
6. The core driver converts raw register bytes into signed axis values.

## Design assumptions

- The I2C peripheral is configured externally and ready for use.
- The project uses STM32H7 CMSIS device register definitions.
- Timing and error-handling are minimal; the implementation is proof-of-concept.
- Register definitions and device addresses are based on the ICM-42686 datasheet.

## Extension points

The following areas are intentionally minimal and can be extended:

- full ICM-42686 configuration register support, including power management and ODR selection
- SPI transport adapter implementation
- retry and recovery logic for transient I2C faults
- IRQ/DMA-based CMSIS I2C transfers
- conversion of raw samples to engineering units (mg / dps)
- integration with a complete STM32Cube build system
