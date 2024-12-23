#include <stdint.h>  // Include this header for uint32_t definition

// APB2ENR (APB2 Peripheral Clock Enable Register)
#define APB2ENR ((volatile uint32_t *) 0x40021018) // define a pointer to the address of the APB2ENR register. 

// offsets used for GPIOA and GPIOC clocks. 
#define RCC_APB2Periph_GPIOA ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOC ((uint32_t)0x00000010)

// base address of GPIOA 
#define GPIOA_BASE 0x40010800

// config register for pins 0 through 7 (lower pins) on GPIO port A. 
#define GPIOA_CRL ((volatile uint32_t *) GPIOA_BASE)
// input data register (IDR) for pins 0 through 15 on GPIO port A, offset is 0x08 for the IDR
#define GPIOA_IDR ((volatile uint32_t *)(GPIOA_BASE + 0x08)) 

// offset for pin 0 on GPIOA (PA0) for the user Button
#define PA0 ((uint32_t) 0x0001) 

// base address of GPIOC 
#define GPIOC_BASE 0x40011000

// config register for pins 8 through 15 (higher pins) on GPIO port C, offset for these pins 0x04
#define GPIOC_CRH ((volatile uint32_t *)(GPIOC_BASE + 0x04)) 
// output data register (ODR) for pins 0 through 15 on GPIO port C, offset for the ODR is 0x0C
#define GPIOC_ODR ((volatile uint32_t *)(GPIOC_BASE + 0x0C)) 


int main(void) {

    // configure peripheral clocks 

    // toggle bits that cooresponds to the peripheral clock for GPIOC and GPIOA
    *APB2ENR |= RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA;

    // configure button 

    // configure button on PA0 to be in Floating input (reset state) mode. To do this we use 0x4 or 0b100. 
    // This will toggle bit 3 on GPIOA, which is 1 of 2 Config bits for GPIO, CNF0[1:0]. When CNF0[1:0] = 01, we are in Floating input mode. 
    // See page 113 of STM32F100x reference manual. 
    *GPIOA_CRL |= 0x4; 

    // configure led

    // clear bits 0 through 3 in the GPIOC_CRH register, this will correspond to CNF8 and MODE8, which are for pin 8 (PC8)
    *GPIOC_CRH &= ~(0xF << 0); 
    // In output mode CNF8[1:0]:
    // 00: General purpose output push-pull
    // clearing the bits in the first step, already puts PC8 in output mode pushpull.
    
    // set the MODE8 bits to be:  10: Output mode, max speed 2 MHz.
    *GPIOC_CRH |= 0x2; // 0x2 = 0b10 in binary
    
    while(1) {

        // Isolate the first bit of the GPIOA_IDR register (uses a bit mask made by 1 << 0 shift operation), 
        // and see if its value is 1. We do this because GPIOA_IDR's first bit is 1 if Pin 0 is high. 
        // if Pin 0 is high, the user button has been pressed. 
        if ((*GPIOA_IDR & (1 << 0)) == 1) {
            *GPIOC_ODR |= (1 << 8); // enable pin 8 (PC8) 
        } 
        else {
            *GPIOC_ODR &= ~(1 << 8); // disable pin 8 (PC8) 
        }
    }
}