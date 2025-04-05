#ifndef I2C_H_
#define I2C_H_

#include <stdint.h>

void I2C1_Init(void);
uint8_t I2C_Read_Register(uint8_t dev_addr, uint8_t reg_addr);
void I2C_Write_Register(uint8_t dev_addr, uint8_t reg_addr, uint8_t data);

#endif /* I2C_H_ */
