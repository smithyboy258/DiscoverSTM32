/*
* Write a program to perform a test of reading/writing individual locations
* as well as block read/write.
*/

#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi.h"
#include "eeprom.h"

/*
Setup:

*/

static uint8_t txbuf[4], rxbuf[4];
static uint16_t txbuf16[4], rxbuf16[4];
// monitor this
static uint8_t status; 

int main() {
    // initialize EEPROM
    eepromInit();

    // read status. 
    status = eepromReadStatus();

    return(0);
}

// Timer code
static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime)
{
    TimingDelay = nTime;
    while(TimingDelay !=0);
}

void SysTick_Handler(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file , uint32_t line)
{
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
#endif