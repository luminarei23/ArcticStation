#include "aqi_sensor_serial.hpp"

// Pico SDK libraries
#include "hardware/resets.h"
#include "hardware/uart.h"

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

void AirSensorStream::init()
{
    // Set up our UART with a basic baud rate for PMS7003.
    uart_init(UART_ID, BAUD_RATE);

    // Set the TX and RX pins by using the function select on the GPIO
    // Set datasheet for more information on function select
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    // Set our data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    printf("uart1 Enabled? %d\n", uart_is_enabled(uart1));
}

bool AirSensorStream::isBusy()
{
    return !(uart_get_hw(UART_ID)->fr & UART_UARTFR_TXFF_BITS);
}

char AirSensorStream::read()
{
    return uart_getc(UART_ID);
}

void AirSensorStream::flush()
{
    while (!isTransmitEmpty())
        ; // Wait for transmit FIFO to be empty
}

bool AirSensorStream::isTransmitEmpty()
{
    return uart_get_hw(UART_ID)->fr & UART_UARTFR_TXFE_BITS;
}

void AirSensorStream::reset()
{
    invalid_params_if(UART, UART_ID != uart0 && UART_ID != uart1);
    reset_block(uart_get_index(UART_ID) ? RESETS_RESET_UART1_BITS : RESETS_RESET_UART0_BITS);
}

void AirSensorStream::unreset()
{
    invalid_params_if(UART, UART_ID != uart0 && UART_ID != uart1);
    unreset_block_wait(uart_get_index(UART_ID) ? RESETS_RESET_UART1_BITS : RESETS_RESET_UART0_BITS);
}