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

#include <stdio.h>
#include <string.h>

/*
Setup:

*/

static uint8_t txbuf[4], rxbuf[4];
static uint16_t txbuf16[4], rxbuf16[4];
// monitor this

int main() {
    // initialize EEPROM
    eepromInit();

    uint8_t test_data_write[16];
    uint8_t test_data_read[16];
    uint16_t start_address = 0x0110;
    int success = 1;

    // 1. Prepare a pattern to write (e.g., 0, 1, 2... 15)
    for (int i = 0; i < 16; i++) {
        test_data_write[i] = (uint8_t)i;
    }

    // 2. Perform the Write
    // Note: We write exactly 16 bytes (one full page)
    if (eepromWrite(test_data_write, 16, start_address)) {
        success = 1; 
    }

    // 3. Perform the Read
    // The eepromRead function includes the WIP check, 
    // so it will wait for the internal write cycle to finish.
    if (eepromRead(test_data_read, 16, start_address)) {
      success = 1;
    }

    // 4. Verify the Data
    for (int i = 0; i < 16; i++) {
        if (test_data_read[i] != test_data_write[i]) {
            success = 0;
            break;
        }
    }

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