/*
 * spi.h
 *
 *  Created on: Mar 30, 2025
 *      Author: khale
 */

#ifndef SPI_H_
#define SPI_H_

#include "stm32f10x.h"

// Initialize SPI1 (PA5=SCK, PA6=MISO, PA7=MOSI)
void SPI1_Init(void);

// Transmit an array of bytes over SPI
void SPI_transmit(uint8_t* data, uint16_t len);


#endif /* SPI_H_ */
