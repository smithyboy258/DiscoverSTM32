#ifndef EEPROM_H
#define EEPROM_H

// Defines for SPI device and Pins used by EEPROM

// port used for the CS select pin
#define EEPROM_PORT GPIOB
// cs select pin, on port B, this is PB12. 
#define EEPROM_CS GPIO_Pin_12
// book says to use an SPI_SLOW setting, which uses a prescaler of 64, to achieve a 0.375MHz SPI clock.
#define EEPROM_SPEED SPI_SLOW
// book says to use SPI device 2 on the STM32F100x
#define EEPROM_SPI SPI2

// enum for EEPROM commands
enum eepromCMD { 
    cmdREAD = 0x03 , cmdWRITE = 0x02 ,
    cmdWREN = 0x06 , cmdWRDI = 0x04 ,
    cmdRDSR = 0x05 , cmdWRSR = 0x01 
};

// initializes the SPI configurations and CS pin for the EEPROM
void eepromInit ();

void eepromWriteEnable ();

void eepromWriteDisable ();

// reads the value of the EEPROM status regiser. 
uint8_t eepromReadStatus ();

void eepromWriteStatus(uint8_t status);

int eepromWrite(uint8_t *buf , uint8_t cnt , uint16_t offset);

int eepromRead(uint8_t *buf , uint8_t cnt , uint16_t offset);

#endif