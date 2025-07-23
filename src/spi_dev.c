// spi_dev.c
#include "spi_dev.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

// “Hello World\n” – 12 bytes
static const uint8_t HELLO[] = "Hello World\n";

void SPIDEV_init(void) {
    // 1) Power on GPIO & USART1 clocks
    CMU_ClockEnable(cmuClock_GPIO,   true);
    CMU_ClockEnable(cmuClock_USART1, true);

    // 2) Configure the four header pins:
    //    B10 -> PA3 = MISO (push-pull output)
    //    A8 -> PA2 = MOSI (input)
    //    A7 -> PA0 = SCLK (input)
    //    B9 -> PA1 = CS   (input with pull-up)

    // A7 → PA0  = SCLK
    GPIO_PinModeSet(gpioPortA, 0, gpioModeInput,    0);

    // B9 → PA1  = CS (pulled-up)
    GPIO_PinModeSet(gpioPortA, 1, gpioModeInputPull, 1);

    // A8 → PA2  = MOSI
    GPIO_PinModeSet(gpioPortA, 2, gpioModeInput,    0);

    // B10 → PA3 = MISO
    GPIO_PinModeSet(gpioPortA, 3, gpioModePushPull, 0);

    // 3) Route USART1 to Location 2 (PA0..PA3) in SPI-mode:
    //    RXPEN=1, TXPEN=1, CLKPEN=1, CSPEN=1, LOC2=(_LOC2<<8)
    USART1->ROUTE = USART_ROUTE_RXPEN
                    | USART_ROUTE_TXPEN
                    | USART_ROUTE_CLKPEN
                    | USART_ROUTE_CSPEN
                    | USART_ROUTE_LOCATION_LOC2;

    // 4) Init as SPI-slave, MSB-first, 1 MHz clock, mode 0
    USART_InitSync_TypeDef cfg = USART_INITSYNC_DEFAULT;
    cfg.master    = false;               // slave mode
    cfg.msbf      = true;                // send MSB first
    cfg.clockMode = usartClockMode0;     // CPOL=0, CPHA=0
    USART_InitSync(USART1, &cfg);

    // 5) Finally enable TX & RX
    USART_Enable(USART1, usartEnable);
    }

    void SPIDEV_sendHello(void) {
    // For each byte, wait for TX buffer empty, then push it
    for (unsigned i = 0; i < sizeof(HELLO) - 1; i++) {
        while (!(USART1->STATUS & USART_STATUS_TXBL)) {
        // spin until TX buffer level is OK
        }
        USART_Tx(USART1, HELLO[i]);
    }
    // Wait for the final bit to shift out
    while (!(USART1->STATUS & USART_STATUS_TXC)) {
        ;
    }
}
