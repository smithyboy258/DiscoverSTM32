#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi.h"

static const uint16_t speeds[] =
{
    [SPI_SLOW] = SPI_BaudRatePrescaler_64,
    [SPI_MEDIUM] =  SPI_BaudRatePrescaler_8,
    [SPI_FAST] = SPI_BaudRatePrescaler_2
};

void spiInit(SPI_TypeDef *SPIx)
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_StructInit(&GPIO_InitStructure);
    SPI_StructInit(&SPI_InitStructure);

    if (SPIx == SPI1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 |RCC_APB2Periph_AFIO | 
            RCC_APB2Periph_GPIOA,ENABLE);

        //SCK and MOSI
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);

        //MISO
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOA,&GPIO_InitStructure);
    }
    
    else if (SPIx == SPI2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);

        //SCK and MOSI
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);

        //MISO
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
        GPIO_Init(GPIOB,&GPIO_InitStructure);
    }

    else
    {
        return;
    }

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = speeds[SPI_SLOW];
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPIx,&SPI_InitStructure);

    SPI_Cmd(SPIx, ENABLE);    
}

int spiReadWrite(SPI_TypeDef *SPIx, uint8_t *rbuf, 
    const uint8_t *tbuf, int cnt, enum spiSpeed speed)
{
    SPIx->CR1 = (SPIx->CR1 & ~SPI_BaudRatePrescaler_256) | speeds[speed];

    for(int i = 0; i<cnt; i++)
    {
        if(tbuf)
        {
            SPI_I2S_SendData(SPIx, *tbuf++); //send bit and move ptr
        }
        else
        {
            SPI_I2S_SendData(SPIx,0xff); //send all 1's if nothing to send
        }

        //wait until we have something in rbuf
        while (SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET);
        
        if (rbuf)
        {
            *rbuf++ = SPI_I2S_ReceiveData(SPIx); //assign to rbuf then move ptr
        }
        else
        {
            SPI_I2S_ReceiveData(SPIx); //recieve but don't assign if no rx
        }
    }
    return 1;
}    

int spiReadWrite16(SPI_TypeDef *SPIx, uint16_t *rbuf, 
    const uint16_t *tbuf, int cnt, enum spiSpeed speed)
{
    SPI_DataSizeConfig(SPIx,SPI_DataSize_16b);
    SPIx->CR1 = (SPIx->CR1 & ~SPI_BaudRatePrescaler_256) | speeds[speed];

    for(int i = 0; i<cnt; i++)
    {
        if(tbuf)
        {
            SPI_I2S_SendData(SPIx, *tbuf++);
        }
        else
        {
            SPI_I2S_SendData(SPIx,0xff);
        }

        while (SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET);

        if (rbuf)
        {
            *rbuf++ = SPI_I2S_ReceiveData(SPIx);
        }
        else
        {
            SPI_I2S_ReceiveData(SPIx);
        }
    }

    SPI_DataSizeConfig(SPIx,SPI_DataSize_8b);

    return 1;
}  

//CS pin init
void csInit()
{
    // configuring chip select (CS) pin, or otherwise called the Slave Select Pin. Disco Board manaul says the hardware NSS pin is PB12, see page 17. 
    // however, in software mode, we can use almost any GPIO pin, the book says to use PC10. 
    // You can still use PB12 as the NSS pin, since its still a GPIO, it will just ignore PB12's IO value. See page 560 of stm32f100xx reference manual for SSI bit
    // in the SPI control register 1 (SPI_CR1). 

    //clock for GPIOB
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

    //CS pin setup
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    //pin specs
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_WriteBit(GPIOB, GPIO_Pin_12, 1);
}
