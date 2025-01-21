#include "stm32f1xx.h"
#include "test_gpio.h"
#include "i2c.h"


int main(void)
{
    i2c_init(I2C_NUM_1, I2C_SM);
    return 0;
}
