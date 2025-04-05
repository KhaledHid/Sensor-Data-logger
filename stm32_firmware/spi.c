#include "spi.h"
#include "stm32f10x_gpio.h"    // GPIO configuration
#include "stm32f10x_spi.h"     // SPI peripheral control
#include "stm32f10x_rcc.h"     // RCC (clock) settings

void SPI1_Init(void)
{
    // Enable clock for SPI1 and GPIOA (used for SPI pins)
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1, ENABLE);

    // Configure SPI1 pins (PA5=SCK, PA6=MISO, PA7=MOSI)
    GPIO_InitTypeDef gpio;

    // Set PA5 (SCK) and PA7 (MOSI) as alternate function push-pull outputs
    gpio.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio);

    // Set PA6 (MISO) as input floating (to receive from slave)
    gpio.GPIO_Pin = GPIO_Pin_6;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio);

    // Configure SPI1 as master
    SPI_InitTypeDef spi;
    spi.SPI_Direction = SPI_Direction_2Lines_FullDuplex;     // Full-duplex
    spi.SPI_Mode = SPI_Mode_Master;                          // Master mode
    spi.SPI_DataSize = SPI_DataSize_8b;                      // 8-bit data
    spi.SPI_CPOL = SPI_CPOL_Low;                             // Clock idle low
    spi.SPI_CPHA = SPI_CPHA_1Edge;                           // Data valid on first edge
    spi.SPI_NSS = SPI_NSS_Soft;                              // Software NSS management
    spi.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;    // Clock divided by 16
    spi.SPI_FirstBit = SPI_FirstBit_MSB;                     // MSB first
    spi.SPI_CRCPolynomial = 7;                               // Default CRC value

    SPI_Init(SPI1, &spi);    // Apply settings
    SPI_Cmd(SPI1, ENABLE);   // Enable SPI1
}

// Transmit data buffer over SPI1
void SPI_Transmit(uint8_t* data, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
        // Wait until TX buffer is ready
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

        // Send one byte
        SPI_I2S_SendData(SPI1, data[i]);

        // Wait for RX buffer to receive response
        while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

        // Dummy read to clear RX buffer
        (void)SPI_I2S_ReceiveData(SPI1);
    }
}
