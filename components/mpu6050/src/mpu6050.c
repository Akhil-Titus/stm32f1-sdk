#include <stdio.h>
#include "mpu6050.h"
#include "stdbool.h"


/** Verify the I2C connection.
 * Make sure the device is connected and responds as expected.
 * @return True if connection is valid, FALSE otherwise
 */
bool mpu6050_test_connection();

/** Get Device ID.
 * This register is used to verify the identity of the device (0b110100).
 * @return Device ID (should be 0x68, 104 dec, 150 oct)
 * @see MPU6050_RA_WHO_AM_I
 * @see MPU6050_WHO_AM_I_BIT
 * @see MPU6050_WHO_AM_I_LENGTH
 */
static uint8_t mpu6050_get_dev_id();


/** Read multiple bits from an 8-bit device register.
 * @param slaveAddr I2C slave device address
 * @param regAddr Register regAddr to read from
 * @param bitStart First bit position to read (0-7)
 * @param length Number of bits to read (not more than 8)
 * @param data Container for right-aligned value (i.e. '101' read from any bitStart position will equal 0x05)
 * @param timeout Optional read timeout in milliseconds (0 to disable, leave off to use default class value in readTimeout)
 */
static void mpu6050_read_bits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data);


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
    return mpu6050_get_dev_id() == 0x34; //0b110100; 8-bit representation in hex = 0x34
}

static uint8_t mpu6050_get_dev_id()
{
    uint8_t tmp;
    mpu6050_read_bits(MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, &tmp);
    return tmp;
}


/**
 * ******************************************************************************************************* 
*/

/**
 * ***********************************************************************************************
 * MPU6050 specific functions
 * 
 */

static void mpu6050_read_bits(uint8_t regAddr, uint8_t bitStart, uint8_t length, uint8_t *data)
{
    uint8_t tmp;
    // Read a single byte from the register
    uint8_t ret = mpu6050_i2c_read(regAddr, &tmp, 1); // Length is 1 since we're reading 1 byte
    // if (ret != 0)
    // {
    //     LogError("MPU6050", "Failed to read from register 0x%x: %d", regAddr, ret);
    //     return;
    // }

    // // Create a mask for the bits we want to extract
    uint8_t mask = ((1 << length) - 1) << (bitStart - length + 1);

    // Mask and shift to extract the desired bits
    tmp &= mask;
    tmp >>= (bitStart - length + 1);

    *data = tmp; // Store the extracted bits in the output variable
}

/**
 * ******************************************************************************************************* 
*/