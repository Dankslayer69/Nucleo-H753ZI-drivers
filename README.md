# Nucleo-H753ZI-drivers
for ICM-42686 gyro



# ICM-42686-P Relevant Memory Addresses

This document contains the essential I2C/SPI register addresses for writing a basic driver for the ICM-42686-P 6-axis IMU. All addresses below are located in User Bank 0.

## Device Address (I2C)
* **`I2C_ADDRESS`**: `0x68` (Default, if AD0 pin is low) or `0x69` (if AD0 pin is high)

## Identification & Power
* **`WHO_AM_I`**: `0x75` 
  * *Expected Value*: `0x44` (Confirms device identity)
* **`PWR_MGMT0`**: `0x4E`
  * *Usage*: Turn on accelerometer and gyroscope, switch power modes.

## Accelerometer Data Registers (16-bit)
Data is split into High (Upper) and Low (Lower) 8-bit bytes. Combine them using bitwise operations (`(High << 8) | Low`).

* **`ACCEL_DATA_X1`** (High Byte): `0x1F`
* **`ACCEL_DATA_X0`** (Low Byte): `0x20`
* **`ACCEL_DATA_Y1`** (High Byte): `0x21`
* **`ACCEL_DATA_Y0`** (Low Byte): `0x22`
* **`ACCEL_DATA_Z1`** (High Byte): `0x23`
* **`ACCEL_DATA_Z0`** (Low Byte): `0x24`

## Gyroscope Data Registers (16-bit)
Data is split into High (Upper) and Low (Lower) 8-bit bytes.

* **`GYRO_DATA_X1`** (High Byte): `0x25`
* **`GYRO_DATA_X0`** (Low Byte): `0x26`
* **`GYRO_DATA_Y1`** (High Byte): `0x27`
* **`GYRO_DATA_Y0`** (Low Byte): `0x28`
* **`GYRO_DATA_Z1`** (High Byte): `0x29`
* **`GYRO_DATA_Z0`** (Low Byte): `0x2A`

## Configuration Registers (Essential for Setup)
* **`ACCEL_CONFIG0`**: `0x50`
  * *Usage*: Set accelerometer full-scale range (e.g., ±2g, ±16g) and Output Data Rate (ODR).
* **`GYRO_CONFIG0`**: `0x4F`
  * *Usage*: Set gyroscope full-scale range (e.g., ±250dps, ±2000dps) and Output Data Rate (ODR).
