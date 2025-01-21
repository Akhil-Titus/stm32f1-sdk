#ifndef I2C_H
#define I2C_H

#include "stm32f1xx.h"

#define I2C_SM 0x2D
#define I2C_FM 0xB4

#define I2C_NUM_1 1
#define I2C_NUM_2 2

#define I2C_WRITE 0
#define I2C_READ 1

#define i2C_ACK 0
#define i2C_NACK 1

void i2c_init(char i2c, unsigned short speed_mode);

void i2c_start(char i2c);

void i2c_address(char i2c, char address, char RW);

void i2c_data_transfer(char i2c, char data);

void i2c_write(char i2c, char address, char data[]);

char i2c_read(char i2c, char ACK_NACK);

void i2c_stop(char i2c);

#endif