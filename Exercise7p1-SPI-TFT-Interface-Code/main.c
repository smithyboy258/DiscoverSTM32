#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi.h"
#include "st7735.h"

int main() {
    // if (SysTick_Config(SystemCoreClock / 1000)) // Every 1 msec, the timer triggers a call to the SysTick_Handler
    //     while(1);

    while(1) {
        // code go here
        fillScreen(BLUE);
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file , uint32_t line) {
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
#endif