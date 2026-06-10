#ifndef ST7735_H
#define ST7735_H
#include <stm32f10x.h>
#include <stm32f10x_spi.h>

// This header file also defines the GPIO Pins and SPI definitions. 
// It also includes the code from Listing 7.2 - st7735 interface from
// the 'Discovering STM32' book. Also includes other useful constants 

//Sample Colors
#define BLACK            0x0000
#define BLUE             0x001F
#define GREEN            0x07E0
#define LBLUE            0x07FF
#define RED              0xF800
#define YELLOW           0xFFE0
#define WHITE            0xFFFF

// Pin Definitions: 
// LCD Port will use GPIO Port C
#define LCD_PORT GPIOC
// data control (DC) assigned to PC2
#define GPIO_PIN_DC GPIO_Pin_2
// LCD chip select (SCE) assigned to PC0
#define GPIO_PIN_SCE GPIO_Pin_0
// LCD Reset assigned to PC1
#define GPIO_PIN_RST GPIO_Pin_1

// Backlight Control Port will be GPIO Port A
#define LCD_PORT_BKL GPIOA
// Backlight Control (BKL) assigned to PA1
#define GPIO_PIN_BKL GPIO_Pin_1

// SPI Definitions: 
// SPI LCD will use the SPI2 port
#define SPILCD SPI2
// SPI speed will use the 'Fast' speed 
#define LCDSPEED SPI_FAST

// st7735 interface - Listing 7.2 from Discovering Disco Book:
#define MADCTLGRAPHICS 0x6
#define MADCTLBMP 0x2
#define ST7735_width 128
#define ST7735_height 160

#define LOW 0
#define HIGH 1
#define LCD_C LOW
#define LCD_D HIGH
#define ST7735_CASET 0x2A
#define ST7735_RASET 0x2B
#define ST7735_MADCTL 0x36
#define ST7735_RAMWR 0x2C
#define ST7735_RAMRD 0x2E
#define ST7735_COLMOD 0x3A
#define MADVAL(x) (((x) << 5) | 8)

void ST7735_setAddrWindow(uint16_t x0 , uint16_t y0 , uint16_t x1 , uint16_t y1 , uint8_t madctl);

void ST7735_pushColor(uint16_t *color , int cnt);

void ST7735_backLight(uint8_t on);

void ST7735_init();

void fillScreen(uint16_t color); 

void Delay(uint32_t nTime);

void SysTick_Handler(void);

#endif