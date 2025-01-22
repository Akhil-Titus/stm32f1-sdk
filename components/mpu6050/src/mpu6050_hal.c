#include "mpu6050_hal.h"

uint8_t i2c_address_mpu6050 = 0;


static uint8_t stm32_i2c_init(bool install, uint8_t dev_address)
{
    i2c_address_mpu6050 = dev_address;
    i2c_init(I2C_NUM_1, I2C_SM);
    return 1;
}

static uint8_t stm32_i2c_write(uint8_t reg, uint8_t data)
{
    i2c_start(I2C_NUM_1);

    i2c_address(I2C_NUM_1, i2c_address_mpu6050, 0); // 0x68 is the primary address
    i2c_data_transfer(I2C_NUM_1, reg); // Control function for a data
    i2c_data_transfer(I2C_NUM_1, data);

    i2c_stop(I2C_NUM_1);

    return 1;
}

static uint8_t stm32_i2c_read(uint8_t reg, uint8_t *data, size_t len)
{
    int i;

    i2c_start(I2C_NUM_1);

    i2c_address(I2C_NUM_1, i2c_address_mpu6050, 0); // 0x68 is the primary address
    i2c_data_transfer(I2C_NUM_1, reg);

    i2c_start(I2C_NUM_1);
    i2c_address(I2C_NUM_1, i2c_address_mpu6050, 1);

    for (i = 0; i < (len - 1); i++)
    {
        data[i] = i2c_read(I2C_NUM_1, i2C_ACK);
    }
    data[i] = i2c_read(I2C_NUM_1, i2C_NACK);
        
    i2c_stop(I2C_NUM_1);

    return 1;
}


mpu6050_hal_t mpu6050_hal = {
    .init = stm32_i2c_init,
    .write = stm32_i2c_write,
    .read = stm32_i2c_read,
};
