#include "StreamWrapper.hpp"

#include "hardware/uart.h"
#include "pico/stdlib.h"

void Stream::init()
{
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
}