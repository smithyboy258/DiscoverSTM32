TEMPLATEROOT = ..

# additional compilation flags
CFLAGS += -O1 -g
ASFLAGS += -g

# project files
OBJS= $(STARTUP) EnableLEDsWithOneGPIOInit.o
OBJS+= stm32f10x_gpio.o stm32f10x_rcc.o peripheral_init.o

# include common make file
include $(TEMPLATEROOT)/Makefile.common

