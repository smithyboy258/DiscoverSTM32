TEMPLATEROOT = ..

# compilation flags for gdb

CFLAGS  = -O1 -g
ASFLAGS = -g 

# object files

OBJS=  $(STARTUP) main.o
OBJS += spi.o eeprom.o stm32f10x_spi.o stm32f10x_gpio.o stm32f10x_rcc.o

# include common make file

include $(TEMPLATEROOT)/Makefile.common
