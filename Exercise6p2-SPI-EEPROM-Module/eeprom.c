#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi.h"
#include "eeprom.h"
#include <stdint.h>
#include <stddef.h>

#define WIP(x) ((x) & 1)

void eepromInit() {
    // call csInit to initlialize the PC10 CS pin. 
    csInit(); // CS initialization
    // call spiInit() with the SPI2 device mapped to the EEPROM_SPI define from eeprom.h. 
    spiInit(EEPROM_SPI);
}

//CS pin init
void csInit()
{
    // configuring chip select (CS) pin, or otherwise called the Slave Select Pin. Disco Board manaul says the hardware NSS pin is PB12, see page 17. 
    // however, in software mode, we can use almost any GPIO pin, the book says to use PC10. 
    // You can still use PB12 as the NSS pin, since its still a GPIO, it will just ignore PB12's IO value. See page 560 of stm32f100xx reference manual for SSI bit
    // in the SPI control register 1 (SPI_CR1). 

    // 4-30-2026 - disregard above comment and use PC10

    //clock for GPIOC
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

    //CS pin setup
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);
    //pin specs
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(EEPROM_PORT,&GPIO_InitStructure);
    GPIO_WriteBit(EEPROM_PORT, GPIO_Pin_10, 1);
}


uint8_t eepromReadStatus() {
    // defines the command to send to the EEPROM over the MOSI pin, contains the read status code, 0x05, 
    // and another byte that sends while status byte is being received into the res buffer. 
    uint8_t cmd[] = {cmdRDSR , 0xff};

    // buffer received back from EEPROM on the MISO pin, will contain empty data in res[0], status will 
    // be obtained in res[1], one byte after the RDSR command was transmitted to the EEPROM. 
    uint8_t res[2];

    GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 0);

    spiReadWrite(EEPROM_SPI , res , cmd , 2, EEPROM_SPEED);

    GPIO_WriteBit(EEPROM_PORT , EEPROM_CS , 1);

    return res[1];
}

void eepromWriteEnable() {
    uint8_t cmd = cmdWREN;

    while (WIP(eepromReadStatus()));

    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 0);

    spiReadWrite(EEPROM_SPI, 0, &cmd, 1, EEPROM_SPEED);

    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 1);
}

void eepromWriteDisable() {
    uint8_t cmd = cmdWRDI;

    while (WIP(eepromReadStatus()));

    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 0);

    spiReadWrite(EEPROM_SPI, 0, &cmd, 1, EEPROM_SPEED);

    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 1);
}

// 3.2 Read Sequence
// The device is selected by pulling CS low. The 8-bit
// READ instruction is transmitted to the 25LCXXX
// followed by the 16-bit address. After the correct READ
// instruction and address are sent, the data stored in the
// memory at the selected address are shifted out on the
// SO pin. The data stored in the memory at the next
// address can be read sequentially by continuing to
// provide clock pulses. The internal Address Pointer is
// automatically incremented to the next higher address
// after each byte of data is shifted out. When the highest
// address is reached, the address counter rolls over to
// address 0000h allowing the read cycle to be continued
// indefinitely. The read operation is terminated by raising
// the CS pin (Figure 3-1).
int eepromRead(uint8_t *buf , uint8_t cnt , uint16_t offset) {
    uint8_t addr_high;
    uint8_t addr_low;

    addr_high = (uint8_t)(offset >> 8);
    addr_low = (uint8_t)(offset & 0xFF);
    uint8_t cmd[] =  {cmdREAD , addr_high, addr_low};

    while (WIP(eepromReadStatus()));

    // 3. Start Transaction
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 0); // CS Low

    // 4. Send the 3-byte header (Instruction + Address)
    // We pass NULL for rbuf because we don't care about 
    // what the EEPROM sends while we are still talking to it.
    spiReadWrite(EEPROM_SPI, NULL, cmd, 3, EEPROM_SPEED);

    // 5. Read the actual data into the user's buffer
    // We pass NULL for tbuf; your function will automatically 
    // send 0xFF (dummy bytes) to clock the data out.
    spiReadWrite(EEPROM_SPI, buf, NULL, cnt, EEPROM_SPEED);

    // 6. End Transaction
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 1); // CS High

    return 1; // Success
}

// 3.3 Write Sequence
// Prior to any attempt to write data to the 25LCXXX, the
// write enable latch must be set by issuing the WREN
// instruction (Figure 3-4). This is done by setting CS low
// and then clocking out the proper instruction into the
// 25LCXXX. After all eight bits of the instruction are
// transmitted, the CS must be brought high to set the
// write enable latch. If the write operation is initiated
// immediately after the WREN instruction without CS
// being brought high, the data will not be written to the
// array because the write enable latch will not have been
// properly set.
// Once the write enable latch is set, the user may
// proceed by setting the CS low, issuing a WRITE instruction,
// followed by the 16-bit address, and then the data
// to be written. Depending upon the density, a page of
// data that ranges from 16 bytes to 64 bytes can be sent
// to the device before a write cycle is necessary. The only
// restriction is that all of the bytes must reside in the
// same page
int eepromWrite(uint8_t *buf , uint8_t cnt , uint16_t offset) {
// 1. Check if the device is busy from a previous operation
    while (WIP(eepromReadStatus()));

    // 2. Set the Write Enable Latch (WREN)
    // The manual states CS MUST go high after WREN to set the latch
    eepromWriteEnable();

    // 3. Prepare the Write Command and Address
    uint8_t addr_high = (uint8_t)(offset >> 8);
    uint8_t addr_low  = (uint8_t)(offset & 0xFF);
    uint8_t cmd[] = {cmdWRITE, addr_high, addr_low};

    // 4. Start the Write Transaction
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 0);

    // 5. Send the Header (Instruction + Address)
    // We pass NULL for rbuf because we don't need the MISO garbage
    spiReadWrite(EEPROM_SPI, NULL, cmd, 3, EEPROM_SPEED);

    // 6. Send the Data Payload
    // We pass NULL for rbuf because we are strictly transmitting
    spiReadWrite(EEPROM_SPI, NULL, buf, cnt, EEPROM_SPEED);

    // 7. End the Transaction
    // CS going high triggers the internal "Write Cycle" (t_wc)
    GPIO_WriteBit(EEPROM_PORT, EEPROM_CS, 1);

    // Optional: Wait for the write to finish before returning
    // This ensures the next command won't fail immediately
    while (WIP(eepromReadStatus()));

    return 1; // Success
}