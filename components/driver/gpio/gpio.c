#include "gpio.h"
#include <stdio.h>



void gpio_init(GPIO_TypeDef *GPIOx, uint32_t pin, uint32_t mode, uint32_t config) 
{
    // Enable the clock for the GPIO port
    if (GPIOx == GPIOA) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    } else if (GPIOx == GPIOB) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    } else if (GPIOx == GPIOC) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    } else if (GPIOx == GPIOD) {
        RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
    }

    // Combine mode and config values
    uint32_t value = (config << 2) | mode;

    // Configure the pin
    if (pin < 8) { // Pins 0-7 use the CRL register
        GPIOx->CRL &= ~(0xF << (pin * 4));       // Clear the 4 bits for the pin
        GPIOx->CRL |= (value << (pin * 4));      // Set the mode and config
    } else { // Pins 8-15 use the CRH register
        pin -= 8;
        GPIOx->CRH &= ~(0xF << (pin * 4));       // Clear the 4 bits for the pin
        GPIOx->CRH |= (value << (pin * 4));      // Set the mode and config
    }
}

int gpio_get_status(GPIO_TypeDef *GPIOx, uint32_t pin)
{
    // Check the input pin state
    if (GPIOx->IDR & (1 << pin))
    {
        return 1; // Pin is high
    }
    return 0; // Pin is low
}

void gpio_set_status(GPIO_TypeDef *GPIOx, uint32_t pin, uint8_t state)
{
    if (state)
    {
        // Set the corresponding bit in the ODR to 1 (high)
        GPIOx->ODR |= (1 << pin);
    }
    else
    {
        // Reset the corresponding bit in the ODR to 0 (low)
        GPIOx->ODR &= ~(1 << pin);
    }
}

void delay_ms(unsigned int time)
{
    for (unsigned int i = 0; i < time; i++)
        for (volatile unsigned int j = 0; j < 2000; j++)
            ;
}
