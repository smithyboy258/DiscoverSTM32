#include "peripheral_init.h"

void Peripheral_Init(void) {
    // initialize GPIO Type Definition Structures
    GPIO_InitTypeDef GPIO_A_InitStructure;
    GPIO_InitTypeDef GPIO_C_Init_Structure;
    
    // Use the Bitwise OR operator "|" to combine the address offsets 
    // for GPIOA and GPIOC in the RCC register
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE); 

    // Configure Pins ..2, LED on PC9 (GPIO Port C, pin 9), configured as an output. 
    GPIO_StructInit(&GPIO_A_InitStructure);
    // combine the address offsets for pin 9 and 8 on GPIOC register using |
    GPIO_A_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
    GPIO_A_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_A_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA , &GPIO_A_InitStructure);

    // configure the user button pin
    GPIO_StructInit(&GPIO_C_Init_Structure);
    GPIO_C_Init_Structure.GPIO_Pin = GPIO_Pin_0;
    GPIO_C_Init_Structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOC , &GPIO_C_Init_Structure);
}
