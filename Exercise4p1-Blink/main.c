#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>

void Delay(uint32_t nTime); // function Delay defined at top of file. 

#define APB2ENR_TEST ((volatile uint32_t *) 0x40021018) // define a pointer to the address of the APB2ENR register. 

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    // Enable Peripheral Clocks ..1 
    /*  
        below line writes to the APB2 Peripheral 
        Clock Enable Register, writing 2b10000 
        to this register adress enables the 
        peripheral clock for GPIOC. 
    */
    uint32_t current_value = *APB2ENR_TEST; // read current value at address '0x40021018'
    *APB2ENR_TEST |= 0x10; // toggle bit that cooresponds to the peripheral clock for GPIOC. 

    // below commented code does the same but uses macros 
    // and the RCC Struct defined in the STD_peripheral 
    // library to access the memory address of APB2ENR. 
    // RCC->APB2ENR |= ((uint32_t)0x00000010);

    // function that enables the clock and accesses 
    // the value using macros defined in the STD_peripheral 
    // library. 
    // RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); // (1)

    // Configure Pins ..2, LED on PC9 (GPIO Port C, pin 9), configured as an output. 
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC , &GPIO_InitStructure);

    // Configure SysTick Timer ..3 

    /* 
    Utilize a “timer tick” to measure the
    passage of time. 

    The Cortex-M3 core used in the STM32 processors has a dedicated
    timer for this function. The timer as a multiple of the system clock (which
    is defined in ticks/second) – here we configure it for 1 msec interrupts (the
    constant SystemCoreClock is defined in the firmware library to be the number
    of system clock cycles per second): 
    */
    if (SysTick_Config(SystemCoreClock / 1000)) // Every 1 msec, the timer triggers a call to the SysTick_Handler
        while(1);

    while (1) {
        static int LEDval = 0;
        // toggle led ..4, pin can be set or reset with the following code:
        GPIO_WriteBit(GPIOC , GPIO_Pin_9 , (LEDval) ? Bit_SET : Bit_RESET);
        LEDval = 1 - LEDval;

        Delay(1000); // wait 250ms
    }
}
// Timer code ..5 
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