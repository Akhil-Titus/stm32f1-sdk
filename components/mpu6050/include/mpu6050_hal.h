#ifndef MPU6050_HAL_H
#define MPU6050_HAL_H

#include <stdio.h>
#include "i2c.h"
#include "gpio.h"
#include "stdbool.h"

typedef struct
{
    uint8_t (*init)(bool install, uint8_t dev_address);
    uint8_t (*write)(uint8_t reg, uint8_t data);
    uint8_t (*read)(uint8_t reg, uint8_t *data, size_t len); // Added `len` parameter for flexibility
} mpu6050_hal_t;

extern mpu6050_hal_t mpu6050_hal;

// // Logging macros
// #define LogError(tag, message, ...) ESP_LOGE(tag, message, ##__VA_ARGS__)
// #define LogWarn(tag, message, ...) ESP_LOGW(tag, message, ##__VA_ARGS__)
// #define LogInfo(tag, message, ...) ESP_LOGI(tag, message, ##__VA_ARGS__)
// #define LogDebug(tag, message, ...) ESP_LOGD(tag, message, ##__VA_ARGS__)


// Logging macros
// Logging macros for bare-metal
#define LogError(tag, message, ...) ((void)0)
#define LogWarn(tag, message, ...) ((void)0)
#define LogInfo(tag, message, ...) ((void)0)
#define LogDebug(tag, message, ...) ((void)0)


#endif // MPU6050_HAL_H
