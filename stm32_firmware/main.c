#include "stm32f10x.h"      // STM32 core definitions
#include <stdio.h>          // For sprintf
#include "uart.h"           // UART helper functions
#include "i2c.h"            // I2C functions for ADXL345

// Simple delay using empty loop with NOPs
void delay(uint32_t time)
{
    while (time--) __NOP();
}

int main(void)
{
    SystemInit();           // Initialize system clocks
    UART_Init();            // Initialize USART1 (9600 baud)
    I2C1_Init();            // Initialize I2C1 (PB6/SCL, PB7/SDA)

    // Configure ADXL345 accelerometer
    I2C_Write_Register(0x53, 0x2D, 0x08);  // Power on, enable measurement mode
    I2C_Write_Register(0x53, 0x31, 0x08);  // Set data format to full-resolution, ±2g
    I2C_Write_Register(0x53, 0x2C, 0x0A);  // Set data rate to 100 Hz
    delay(1000000);                        // Delay for startup

    while (1)
    {
        // ----- X-axis data -----
        uint8_t x_lsb = I2C_Read_Register(0x53, 0x32);      // Read X-axis LSB
        uint8_t x_msb = I2C_Read_Register(0x53, 0x33);      // Read X-axis MSB
        int16_t raw_x = (int16_t)((x_msb << 8) | x_lsb);    // Combine into signed 16-bit value
        char buf_x[32];
        sprintf(buf_x, "X: %d\r\n", raw_x);                 // Format as string
        UART_SendString(buf_x);                             // Send over UART
        delay(100000);

        // ----- Y-axis data -----
        uint8_t y_lsb = I2C_Read_Register(0x53, 0x34);
        uint8_t y_msb = I2C_Read_Register(0x53, 0x35);
        int16_t raw_y = (int16_t)((y_msb << 8) | y_lsb);
        char buf_y[32];
        sprintf(buf_y, "Y: %d\r\n", raw_y);
        UART_SendString(buf_y);
        delay(100000);

        // ----- Z-axis data -----
        uint8_t z_lsb = I2C_Read_Register(0x53, 0x36);
        uint8_t z_msb = I2C_Read_Register(0x53, 0x37);
        int16_t raw_z = (int16_t)((z_msb << 8) | z_lsb);
        char buf_z[32];
        sprintf(buf_z, "Z: %d\r\n", raw_z);
        UART_SendString(buf_z);
        delay(100000);

        // Separator between samples
        UART_SendString("------------------------\r\n");
        delay(500000);
    }
}
