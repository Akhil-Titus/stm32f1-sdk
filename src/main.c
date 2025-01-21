#include "stm32f1xx.h"
#include "test_gpio.h"
#include "i2c.h"

char new_data[2] = {0x01,0x02};

int main(void)
{
    i2c_init(I2C_NUM_1, I2C_SM);
    i2c_write(1, 0x3C, new_data);
    return 0;
}
