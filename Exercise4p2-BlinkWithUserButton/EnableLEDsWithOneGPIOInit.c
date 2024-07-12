#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

#include "peripheral_init.h"

void Delay(uint32_t nTime); // function Delay defined at top of file. 

int main(void)
{
    // Moved all Peripheral Clock and GPIO Port and Pin 
    // configuration to this helper function to make code
    // look cleaner. 
    Peripheral_Init();

    // Configure SysTick Timer
    if (SysTick_Config(SystemCoreClock / 1000)) // Every 1 msec, the timer triggers a call to the SysTick_Handler
        while(1);

    while (1) {
        static int LEDval = 0;
        // toggle led, pin can be set or reset with the following code:
        GPIO_WriteBit(GPIOC , GPIO_Pin_9 , (LEDval) ? Bit_SET : Bit_RESET);
        LEDval = 1 - LEDval;

        // read user button (Pin PA0) and toggle pin PC8 to turn on LED4 if user button state is 1 (pressed)
        if((GPIO_ReadInputDataBit(GPIOA , GPIO_Pin_0)) == 1)
            GPIO_WriteBit(GPIOC , GPIO_Pin_8 , Bit_SET);
        else 
            GPIO_WriteBit(GPIOC , GPIO_Pin_8 , Bit_RESET);

        Delay(1000); // wait 1s
    }
}
// Timer code
static __IO uint32_t TimingDelay;

void Delay(uint32_t nTime) { // delay code defined here
    TimingDelay = nTime;
    while(TimingDelay != 0);
}

void SysTick_Handler(void) {
    if (TimingDelay != 0x00)
        TimingDelay --;
}

#ifdef USE_FULL_ASSERT
    void assert_failed(uint8_t* file , uint32_t line)
    {
    /* Infinite loop */
    /* Use GDB to find out why we're here */
    while (1);
    }
#endif