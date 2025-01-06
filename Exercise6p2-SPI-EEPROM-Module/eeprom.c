#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_spi.h>
#include "spi.h"
#include "eeprom.h"

void eepromInit() {
    // call csInit to initlialize the PB12 CS pin. 
    csInit(); // CS initialization
    // call spiInit() with the SPI2 device mapped to the EEPROM_SPI define from eeprom.h. 
    spiInit(EEPROM_SPI);
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

