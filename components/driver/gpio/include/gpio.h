#ifndef GPIO_H
#define GPIO_H

#include "stm32f1xx.h"

// GPIO directions
#define GPIO_MODE_INPUT      0     // 00: Input mode (reset state)
#define GPIO_MODE_OUTPUT_10  1     // 01: Output mode, max speed 10 MHz
#define GPIO_MODE_OUTPUT_2  2     // 10: Output mode, max speed 2 MHz
#define GPIO_MODE_OUTPUT_50  3     // 11: Output mode, max speed 50 MHz

// GPIO configuration for input
#define GPIO_IP_AM           0     // 00: Analog mode
#define GPIO_IP_FI           1     // 01: Floating input (reset state)
#define GPIO_IP_PU_PD        2     // 10: Input with pull-up/pull-down

// GPIO configuration for output
#define GPIO_OP_PP           0     // 00: General-purpose output push-pull
#define GPIO_OP_OD           1     // 01: General-purpose output open-drain
#define GPIO_OP_AF_PP        2     // 10: Alternate function output push-pull
#define GPIO_OP_AF_OD        3     // 11: Alternate function output open-drain

#define GPIO_LOW 0
#define GPIO_HIGH 1


/**
 * @brief Initialize a GPIO pin with specified mode and configuration.
 * 
 * @param GPIOx GPIO port (e.g., GPIOA, GPIOB, etc.)
 * @param pin Pin number (0-15)
 * @param mode Mode of the pin (e.g., input/output/speed)
 * @param config Configuration of the pin (e.g., push-pull, open-drain)
 * 
 * This function configures a GPIO pin by resetting its current configuration and applying
 * the specified mode and configuration values.
 * 
 * - Reset the pin's configuration bits (4 bits per pin).
 * 
 * - `pin * 4` calculates the position in the register for the target pin's 4 bits.
 * 
 * - `0xF (1111)` creates a mask to target the 4 bits for that pin.
 * 
 * - `&= ~` clears the 4 bits of the target pin, leaving other bits unaffected.
 * 
 * - The desired mode and configuration are then applied by OR'ing the value into the register.
 */
void gpio_init(GPIO_TypeDef *GPIOx, uint32_t pin, uint32_t mode, uint32_t config);

/**
 * @brief Read the state of a GPIO pin.
 * 
 * @param GPIOx GPIO port (e.g., GPIOA, GPIOB, etc.)
 * @param pin Pin number (0-15)
 * 
 * @return Pin state (0 for low, 1 for high).
 */
int gpio_get_status(GPIO_TypeDef *GPIOx, uint32_t pin);

/**
 * @brief Set a GPIO pin to a specified state (high or low).
 * 
 * @param GPIOx GPIO port (e.g., GPIOA, GPIOB, etc.)
 * @param pin Pin number (0-15)
 * @param state Pin state (0 for low, 1 for high)
 */
void gpio_set_status(GPIO_TypeDef *GPIOx, uint32_t pin, uint8_t state);

/**
 * @brief Basic delay function
 * 
 * @param time 1000ms = 1s
 */
void delay_ms(unsigned int time);

#endif