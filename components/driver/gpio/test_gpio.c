
#include "test_gpio.h"

void test_gpio(void)
{
    gpio_init(GPIOA, 0, GPIO_MODE_INPUT, GPIO_IP_PU_PD);
    gpio_init(GPIOB, 0, GPIO_MODE_OUTPUT_50, GPIO_OP_PP);
    gpio_init(GPIOC, 13, GPIO_MODE_OUTPUT_50, GPIO_OP_PP);

    while (1)
    {
        if (gpio_get_status(GPIOA, 0))
        {
            // LED in C13 works in opposite
            gpio_set_status(GPIOC, 13, GPIO_HIGH);
            gpio_set_status(GPIOB, 0, GPIO_HIGH);
            delay_ms(500);
            gpio_set_status(GPIOC, 13, GPIO_LOW);
            gpio_set_status(GPIOB, 0, GPIO_LOW);
            delay_ms(500);
        }
    }
}
