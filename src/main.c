#include "stm32f1xx.h"
#include "test_gpio.h"
#include "i2c.h"
#include "mpu6050.h"
#include <stdbool.h>

bool tester_fun(int a, int b)
{
	return a > b;
}



int main(void)
{
	bool connection_test = 0;

	mpu6050_i2c_init(1);
	connection_test = mpu6050_test_connection();
	// connection_test = tester_fun(3, 2);

	if (connection_test)
	{
		while (1)
		{
		};
	}
	else
	{
		return 0;
	}
}
