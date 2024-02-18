#ifndef _STREAMWRAPPER_HPP_
#define _STREAMWRAPPER_HPP_

#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

class Stream
{
public:
    Stream();

    // Set up our UART with the required speed.
    void init();

    size_t write(uint8_t byte);
    int available();
    void flush();

    ~Stream();
};

#endif