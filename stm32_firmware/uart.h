
#ifndef UART_H_
#define UART_H_

#include "stm32f10x.h"

void UART_Init(void);
void UART_SendChar(char c);
void UART_SendString(const char* str);

#endif /* UART_H_ */
