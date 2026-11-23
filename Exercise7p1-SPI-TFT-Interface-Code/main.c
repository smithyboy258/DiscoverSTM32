#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi.h"
#include "st7735.h"

int main() {
    // if (SysTick_Config(SystemCoreClock / 1000)) // Every 1 msec, the timer triggers a call to the SysTick_Handler
    //     while(1);
    ST7735_init();
    ST7735_backLight(1);
    
    while(1) {
        // code go here
        fillScreen(RED);
        Delay(1000);
        fillScreen(GREEN);
        Delay(1000);
        fillScreen(BLUE);
        Delay(1000);
        fillScreen(WHITE);
        drawString(10, 80, "RGB test complete", DGREEN, WHITE);
        Delay(10000);
    }

// Draw String Test Exercise 7.2, TFT display can display a 337 character string
    // drawString(0, 0,
    // "0123456789ABCDEFGHIJK"
    // "LMNOPQRSTUVWXYZ012345"
    // "6789ABCDEFGHIJKLMNOPQ"
    // "RSTUVWXYZ0123456789AB"
    // "CDEFGHIJKLMNOPQRSTUVWXY"
    // "Z0123456789ABCDEFGHIJ"
    // "KLMNOPQRSTUVWXYZ01234"
    // "56789ABCDEFGHIJKLMNOP"
    // "QRSTUVWXYZ0123456789A"
    // "BCDEFGHIJKLMNOPQRSTUV"
    // "WXYZ0123456789ABCDEFG"
    // "HIJKLMNOPQRSTUVWXYZ01"
    // "23456789ABCDEFGHIJKLM"
    // "NOPQRSTUVWXYZ01234567"
    // "89ABCDEFGHIJKLMNOPQRS"
    // "TUVWXYZ0123456789AB",
    // WHITE, BLACK);

}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file , uint32_t line) {
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
}
#endif