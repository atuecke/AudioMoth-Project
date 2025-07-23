/* spi_dev.h – tiny wrapper for USART1-in-SPI mode on AudioMoth-Dev */
#pragma once
#include <stdint.h>
void SPIDEV_init(void);       // call once at boot
void SPIDEV_sendHello(void);  // blocking send of "Hello World\n"