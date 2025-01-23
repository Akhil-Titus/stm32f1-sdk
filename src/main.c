#include "stm32f1xx.h"
#include "test_gpio.h"
#include "i2c.h"
#include "mpu6050.h"
#include <stdbool.h>


int main(void)
{
	bool connection_test = 0;

	mpu6050_i2c_init(1);
	connection_test = mpu6050_test_connection();

	if (connection_test)
	{
		mpu6050_initialize();
	}
	else
	{
		return 0;
	}
}



// Issue is time... when slowed down.. it works