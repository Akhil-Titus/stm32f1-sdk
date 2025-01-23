#include <stdio.h>
#include "mpu6050.h"
#include "stdbool.h"
#include "mpu6050_reg.h"



/** Read multiple bits from an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in readTimeout)
 */
static void mpu6050_read_bits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);

/** Read a single bit from an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitNum Bit position to read (0-7)
 * @param data Container for single bit value
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in readTimeout)
 */

/** write a single bit in an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to write to
 * @param bitNum Bit position to write (0-7)
 * @param value New bit value to write
 */
static void mpu6050_write_bit(uint8_t regAddr, uint8_t bitNum, uint8_t data);

/**
 * ***********************************************************************************************
 * HAL functions
 */
void mpu6050_i2c_init(bool i2c_install)
{
    mpu6050_hal.init(i2c_install, MPU6050_DEFAULT_ADDRESS);
}

static uint8_t mpu6050_i2c_write(uint8_t reg, uint8_t data)
{
    return mpu6050_hal.write(reg, data);
}

static uint8_t mpu6050_i2c_read(uint8_t reg, uint8_t *data, size_t len)
{
    return mpu6050_hal.read(reg, data, len);
}

/**
 * ******************************************************************************************************* 
*/

/**
 * ***********************************************************************************************
 * Test functions
 */

bool mpu6050_test_connection()
{
    uint8_t tmp;
    mpu6050_read_bits(MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, &tmp);
    return tmp == 0x34; //0b110100; 8-bit representation in hex = 0x34
}

/**
 * ******************************************************************************************************* 
*/

/**
 * ***********************************************************************************************
 * MPU6050 specific read and write
 * 
 */

static void mpu6050_read_bits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data)
{
    uint8_t tmp;
    // Read a single byte from the register
    uint8_t ret = mpu6050_i2c_read(regAddr, &tmp, 1); // Length is 1 since we're reading 1 byte
    if (!ret)
    {
        // LogError("MPU6050", "Failed to read from register 0x%x: %d", regAddr, ret);
        return;
    }

    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1); // Create a mask for the bits we want to extract

    tmp &= mask; // Mask and shift to extract the desired bits
    tmp >>= (bitStart - length + 1);

    *data = tmp; // Store the extracted bits in the output variable
}

void MPU6050_ReadBit(uint8_t regAddr, uint8_t bitNum, uint8_t *data)
{
    uint8_t tmp;
    uint8_t ret = mpu6050_i2c_read(regAddr, &tmp, 1); // Length is 1 since we're reading 1 byte
    if (!ret)
    {
        // LogError("MPU6050", "Failed to read from register 0x%x: %d", regAddr, ret);
        return;
    }

    *data = tmp & (1 << bitNum);
}

static void mpu6050_write_bits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t data)
{
    uint8_t tmp;
    uint8_t ret = mpu6050_i2c_read(regAddr, &tmp, 1);
    if (!ret)
    {
        return;
    }
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);
    data <<= (bitStart - length + 1); // shift data into correct position
    data &= mask;                     // zero all non-important bits in data
    tmp &= ~(mask);                   // zero all important bits in existing byte
    tmp |= data;                      // combine data with existing byte

    ret = mpu6050_i2c_write(regAddr, data);
    if (!ret)
    {
        return;
    }
}

static void mpu6050_write_bit(uint8_t regAddr, uint8_t bitNum, uint8_t data)
{
    uint8_t tmp;
    uint8_t ret = mpu6050_i2c_read(regAddr, &tmp, 1);
    if (!ret)
    {
        return;
    }
    tmp = (data != 0) ? (tmp | (1 << bitNum)) : (tmp & ~(1 << bitNum));
    ret = mpu6050_i2c_write(regAddr, tmp);
    if (!ret)
    {
        return;
    }
}

/**
 * ******************************************************************************************************* 
*/


/**
 * ***********************************************************************************************
 * MPU6050 Init
 * 
 */

void mpu6050_initialize()
{
    mpu6050_set_clock_source(MPU6050_CLOCK_PLL_XGYRO);
    mpu6050_set_full_scale_gyro_range(MPU6050_GYRO_FS_250);
    mpu6050_set_full_scale_accel_range(MPU6050_ACCEL_FS_2);
    mpu6050_set_sleep_mode_status(DISABLE);
}


void mpu6050_set_clock_source(uint8_t source)
{
    mpu6050_write_bits(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
}

void mpu6050_set_full_scale_gyro_range(uint8_t range)
{
    mpu6050_write_bits(MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

void mpu6050_set_full_scale_accel_range(uint8_t range)
{
    mpu6050_write_bits(MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

void mpu6050_set_sleep_mode_status(FunctionalState NewState)
{
    mpu6050_write_bit(MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, NewState);
}

/**
 * ******************************************************************************************************* 
*/
