#include "i2c.h"
#include "gpio.h"


// i2c1 is connected to apb1 
// but peripherals can only be activated afted apb2 enable (selecting mode like alternate function)
void i2c_init(char i2c, unsigned short speed_mode)
{
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN; // enable alternate function

    if (i2c == 1)
    {
        RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; // enable i2c1

        /*enable gpio o/p AF open-drain*/
        // PB6 mode 01, config mode 11
        // GPIOB->CRL |= GPIO_CRL_MODE6_0;
        // GPIOB->CRL &= ~GPIO_CRL_MODE6_1;
        // GPIOB->CRL |= GPIO_CRL_CNF6_0 | GPIO_CRL_CNF6_1; // Open drain alternate  config mode 11

        // // PB7 mode 01, config mode 11
        // GPIOB->CRL |= GPIO_CRL_MODE7_0;
        // GPIOB->CRL &= ~GPIO_CRL_MODE7_1;
        // GPIOB->CRL |= GPIO_CRL_CNF7_0 | GPIO_CRL_CNF7_1; // Open drain alternate config mode 11
        
        gpio_init(GPIOB,  6, GPIO_MODE_OUTPUT_10, GPIO_OP_AF_OD);
        gpio_init(GPIOB,  7, GPIO_MODE_OUTPUT_10, GPIO_OP_AF_OD);

        I2C1->CR1 |= I2C_CR1_SWRST; /*enter reset mode*/
        I2C1->CR1 &= ~I2C_CR1_SWRST; /*come out of reset mode*/
        
        // I2C1->CR2 = I2C_CR2_FREQ_3; /*Set peripheral clock freq*/
        I2C1->CR2 = I2C_CR2_FREQ_3;
        I2C1->CCR = speed_mode;
        I2C1->TRISE = I2C_TRISE_TRISE; // since that is the selected clock freq (mentioned in data sheet)
        I2C1->CR1 |= I2C_CR1_PE;  /*Peripheral enable*/
    }
    else if (i2c == 2)
    {
        RCC->APB1ENR |= RCC_APB1ENR_I2C2EN; // enable i2c1

        /*enable gpio o/p AF open-drain*/
        // PB6 mode 01, config mode 11
        GPIOB->CRH |= GPIO_CRH_MODE10_0;
        GPIOB->CRH &= ~GPIO_CRH_MODE10_1;
        GPIOB->CRH |= GPIO_CRH_CNF10_0 | GPIO_CRH_CNF10_1; // Open drain alternate  config mode 11

        // PB7 mode 01, config mode 11
        GPIOB->CRH |= GPIO_CRH_MODE11_0;
        GPIOB->CRH &= ~GPIO_CRH_MODE11_1;
        GPIOB->CRH |= GPIO_CRH_CNF11_0 | GPIO_CRH_CNF11_1; // Open drain alternate config mode 11

        I2C2->CR1 |= I2C_CR1_SWRST; /*enter reset mode*/
        I2C2->CR1 &= ~I2C_CR1_SWRST; /*come out of reset mode*/
        
        // I2C1->CR2 = I2C_CR2_FREQ_3; /*Set peripheral clock freq*/
        I2C2->CR2 = 0x8;
        I2C2->CCR = speed_mode;
        I2C2->TRISE = 0x9; // since that is the selected clock freq (mentioned in data sheet)
        I2C2->CR1 |= I2C_CR1_PE;  /*Peripheral enable*/
    }
}

void i2c_start(char i2c)
{
    if (i2c == 1)
    {
        I2C1->CR1 |= I2C_CR1_START;
        while (!(I2C1->SR1 & 1))
        {
        }; // wait
    }
    else if (i2c == 2)
    {
        I2C2->CR1 |= I2C_CR1_START;
        while (!(I2C2->SR1 & 1))
        {
        }; // wait
    }
}

// sending address + R/W
void i2c_address(char i2c, char address, char RW)
{
    volatile int tmp;
    if (i2c == 1)
    {
        I2C1->DR = (address | RW);
        // I2C1->DR = (address << 1) | RW;

        while (!(I2C1->SR1 & I2C_SR1_ADDR))
        {
        }; /* wait until addr flag is set*/

        while ((I2C1->SR1 & I2C_SR1_ADDR))
        {
            tmp = I2C1->SR1;
            tmp = I2C1->SR2;
            if ((I2C1->SR1 & I2C_SR1_ADDR))
            {
                break;
            }
        }
    }
    else if (i2c == 2)
    {
        I2C1->DR = (address | RW);

        while (!(I2C2->SR1 & I2C_SR1_ADDR))
        {
        }; /* wait until addr flag is set*/

        while ((I2C2->SR1 & I2C_SR1_ADDR))
        {
            tmp = I2C2->SR1;
            tmp = I2C2->SR2;
            if ((I2C2->SR1 & I2C_SR1_ADDR))
            {
                break;
            }
        }
    }
}

void i2c_data_transfer(char i2c, char data)
{
    if (i2c == 1)
    {
        while (!(I2C1->SR1 & I2C_SR1_TXE))
        {
        };
        I2C1->DR = data;
        while (!(I2C1->SR1 & I2C_SR1_TXE))
        {
        };
    }
    else if (i2c == 2)
    {
        while (!(I2C2->SR1 & I2C_SR1_TXE))
        {
        };
        I2C2->DR = data;
        while (!(I2C2->SR1 & I2C_SR1_TXE))
        {
        };
    }
}

void i2c_stop(char i2c)
{
    volatile int tmp;
    if (i2c == 1)
    {
        tmp = I2C1->SR1;
        tmp = I2C1->SR2; // just in case
        I2C1->CR1 |= I2C_CR1_STOP;
    }
    else if (i2c == 2)
    {
        tmp = I2C2->SR1;
        tmp = I2C2->SR2; // just in case
        I2C2->CR1 |= I2C_CR1_STOP;
    }
}

void i2c_write(char i2c, char address, char data[])
{
    int i = 0;
    i2c_start(i2c);
    i2c_address(i2c, address, 0);
    while (data[i])
    {
        i2c_data_transfer(i2c, data[i]);
        i++;
    }
    i2c_stop(i2c);
}

uint8_t i2c_read(uint8_t i2c, uint8_t ACK_NACK)
{
    uint8_t temp;
    if (i2c == 1)
    {
        I2C1->CR1 |= I2C_CR1_ACK;
        while (!(I2C1->SR1 & I2C_SR1_RXNE))
        {
        };
        temp = I2C1->DR;
        if (ACK_NACK)
        {
            I2C1->CR1 &= ~I2C_CR1_ACK;
        }
    }
    else if (i2c == 2)
    {
        I2C2->CR1 |= I2C_CR1_ACK;
        while (!(I2C2->SR1 & I2C_SR1_RXNE))
        {
        };
        temp = I2C2->DR;
        if (ACK_NACK)
        {
            I2C2->CR1 &= ~I2C_CR1_ACK;
        }
    }
    return temp;
}