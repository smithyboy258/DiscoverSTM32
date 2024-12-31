#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi.h"

uint8_t txbuf [4], rxbuf [4];
uint16_t txbuf16 [4], rxbuf16 [4];

void csInit(); 

void main() {  
  int i, j;
  csInit(); // Initialize chip select PC03
  spiInit(SPI2);

  for (i = 0; i < 8; i++) {

    for (j = 0; j < 4; j++)
      txbuf[j] = i*4 + j;

    GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
    spiReadWrite(SPI2 , rxbuf , txbuf , 4, SPI_SLOW);
    GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 1);

    for (j = 0; j < 4; j++)
      if (rxbuf[j] != txbuf[j])
        assert_failed(__FILE__ , __LINE__);
  }

  for (i = 0; i < 8; i++) {

    for (j = 0; j < 4; j++)
      txbuf16[j] = i*4 + j + (i << 8);

    GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 0);
    spiReadWrite16(SPI2 , rxbuf16 , txbuf16 , 4, SPI_SLOW);
    GPIO_WriteBit(GPIOC , GPIO_Pin_3 , 1);

    for (j = 0; j < 4; j++)
      if (rxbuf16[j] != txbuf16[j])
        assert_failed(__FILE__ , __LINE__);
  }

}

// configuring chip select (CS) pin, or otherwise called the Slave Select Pin. Disco Board manaul says the hardware NSS pin is PB12, see page 17. 
// however, in software mode, we can use almost any GPIO pin, the book says to use PC3. 
// You can still use PB12 as the NSS pin, since its still a GPIO, it will just ignore PB12's IO value. See page 560 of stm32f100xx reference manual for SSI bit
// in the SPI control register 1 (SPI_CR1). 
void csInit() {
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_StructInit(&GPIO_InitStructure);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 

  // SCK and MOSI config as general purpose pushpull, output mode, maximum speed 50MHz
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // this would be GPIO_Mode_AF_PP if NSS SPI_NSS_Hard was configured. 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB , &GPIO_InitStructure);

  // Making the Chip select pin high so it can be lowered later to select a device. 
  GPIO_WriteBit(GPIOB , GPIO_Pin_3 , 1);
}

#ifdef USE_FULL_ASSERT
    void assert_failed(uint8_t* file , uint32_t line)
    {
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
    }
#endif