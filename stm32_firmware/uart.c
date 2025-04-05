#include "stm32f10x_gpio.h"     // GPIO configuration
#include "stm32f10x_usart.h"    // USART1 peripheral control
#include "stm32f10x_rcc.h"      // RCC (clock) management
#include "uart.h"               // UART header

// Initialize USART1 for 9600 baud, 8N1 format
void UART_Init(void)
{
    // Enable clock for GPIOA and USART1 peripheral
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitTypeDef gpio;

    // PA9 = TX: set as alternate function push-pull
    gpio.GPIO_Pin = GPIO_Pin_9;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio);

    // PA10 = RX: set as floating input
    gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &gpio);

    // Configure USART1 parameters
    USART_InitTypeDef uart;
    uart.USART_BaudRate = 9600;
    uart.USART_WordLength = USART_WordLength_8b;
    uart.USART_StopBits = USART_StopBits_1;
    uart.USART_Parity = USART_Parity_No;
    uart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    uart.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    USART_Init(USART1, &uart);     // Apply settings
    USART_Cmd(USART1, ENABLE);     // Enable USART1
}

// Send a single character via UART
void UART_SendChar(char c)
{
    // Wait until TX buffer is empty
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    USART_SendData(USART1, c);  // Send character
}

// Send a string over UART
void UART_SendString(const char* str)
{
    while (*str) {
        UART_SendChar(*str++);  // Send characters one by one
    }
}
