#include "i2c.h"                          // Include the corresponding header file for I2C
#include "stm32f10x_gpio.h"              // Include GPIO control definitions
#include "stm32f10x_i2c.h"               // Include I2C peripheral definitions
#include "stm32f10x_rcc.h"               // Include clock configuration definitions

// Initializes I2C1 peripheral using GPIOB pins (PB6 = SCL, PB7 = SDA)
void I2C1_Init(void)
{
    // Enable clock for GPIOB to configure I2C pins
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    // Enable clock for I2C1 peripheral
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // Configure GPIO pins for I2C: PB6 = SCL, PB7 = SDA
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;      // Select SCL and SDA pins
    gpio.GPIO_Speed = GPIO_Speed_50MHz;           // Set GPIO speed to 50MHz
    gpio.GPIO_Mode = GPIO_Mode_AF_OD;             // Set to alternate function open-drain
    GPIO_Init(GPIOB, &gpio);                      // Apply GPIO settings

    // Configure the I2C1 peripheral
    I2C_InitTypeDef i2c;
    i2c.I2C_ClockSpeed = 100000;                  // Set I2C clock to 100kHz (standard mode)
    i2c.I2C_Mode = I2C_Mode_I2C;                  // Set mode to I2C (not SMBus)
    i2c.I2C_DutyCycle = I2C_DutyCycle_2;          // Use 50% duty cycle (standard)
    i2c.I2C_OwnAddress1 = 0x00;                   // Master mode doesn't need a specific address
    i2c.I2C_Ack = I2C_Ack_Enable;                 // Enable acknowledgment for reception
    i2c.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit; // Use 7-bit addressing

    // Apply I2C configuration
    I2C_Init(I2C1, &i2c);

    // Enable I2C1 peripheral
    I2C_Cmd(I2C1, ENABLE);
}

// Reads a single byte from a given register of a device using I2C
uint8_t I2C_Read_Register(uint8_t dev_addr, uint8_t reg_addr)
{
    // Wait until I2C bus is free
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    // Generate START condition
    I2C_GenerateSTART(I2C1, ENABLE);

    // Wait for master mode selection
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    // Send 7-bit device address with write intention (last bit = 0)
    I2C_Send7bitAddress(I2C1, dev_addr << 1, I2C_Direction_Transmitter);

    // Wait for transmitter mode to be selected
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // Send the register address to be read
    I2C_SendData(I2C1, reg_addr);

    // Wait until the byte is transmitted
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Generate a repeated START condition for read
    I2C_GenerateSTART(I2C1, ENABLE);

    // Wait for master mode selection again
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    // Send 7-bit device address with read intention (last bit = 1)
    I2C_Send7bitAddress(I2C1, dev_addr << 1, I2C_Direction_Receiver);

    // Wait for receiver mode to be selected
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    // Wait until data byte is received
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED));

    // Read the received data byte
    uint8_t value = I2C_ReceiveData(I2C1);

    // Generate STOP condition to end the transmission
    I2C_GenerateSTOP(I2C1, ENABLE);

    return value; // Return the received byte
}

// Writes a single byte to a device register over I2C
void I2C_Write_Register(uint8_t dev_addr, uint8_t reg_addr, uint8_t data)
{
    // Wait until I2C bus is free
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    // Generate START condition
    I2C_GenerateSTART(I2C1, ENABLE);

    // Wait for master mode selection
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    // Send 7-bit device address with write intention
    I2C_Send7bitAddress(I2C1, dev_addr << 1, I2C_Direction_Transmitter);

    // Wait for transmitter mode
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    // Send the register address to write to
    I2C_SendData(I2C1, reg_addr);

    // Wait until the byte is transmitted
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Send the data byte to write into the register
    I2C_SendData(I2C1, data);

    // Wait until the data is transmitted
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    // Generate STOP condition to complete transmission
    I2C_GenerateSTOP(I2C1, ENABLE);
}
