#ifndef SPI_H
#define SPI_H

#include <stm32f10x.h>
#include <stm32f10x_spi.h>

enum spiSpeed { SPI_SLOW , SPI_MEDIUM, SPI_FAST };

void spiInit(SPI_TypeDef* SPIx);
int spiReadWrite(SPI_TypeDef* SPIx, uint8_t *rbuf, 
		 const uint8_t *tbuf, int cnt, 
		 enum spiSpeed speed);
int spiReadWrite16(SPI_TypeDef* SPIx, uint16_t *rbuf, 
                   const uint16_t *tbuf, int cnt, 
                   enum spiSpeed speed);

// adding chip select pin initialization, I will just make it use PB12. 
void csInit(void);

#endif
