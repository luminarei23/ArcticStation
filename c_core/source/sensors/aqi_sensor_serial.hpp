#ifndef AQISENSORSERIAL_HPP
#define AQISENSORSERIAL_HPP

// Standard libraries
#include <stdint.h>

// Pico SDK libraries
#include "hardware/uart.h"

#define BAUD_RATE 9600
#define UART_ID uart1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY UART_PARITY_NONE

// pins 0 and 1 from PICO W documentation
#define UART_TX_PIN 5
#define UART_RX_PIN 6

/**
 * @brief A class representing a UART stream for communication.
 * The purpose is to have data read and written on uart 1 instance (PIN 6 and PIN 7)
 * of RPI Pico (for now uart0 is occupied by picoprobe /dev/ttyACM0) PMS7003 Will be
 * occupying this UART instance
 */
class AirSensorStream
{
public:
    /**
     * @brief Constructs a new Stream object with UART0 by default.
     */
    AirSensorStream() noexcept = default;

    /**
     * @brief Initializes the UART stream with the specified baud rate.
     *
     */
    void init() noexcept;

    /**
     * @brief Checks if the UART transmit FIFO is full.
     *
     * @return True if the transmit FIFO is full, otherwise false.
     */
    bool isBusy();

    /**
     * @brief Reads a byte from the UART stream.
     *
     * @return The byte read from the stream.
     */
    char read();

    /**
     * @brief Flushes the transmit FIFO of the UART stream.
     */
    void flush();

    /**
     * @brief Checks if the UART transmit FIFO is empty.
     *
     * @return True if the transmit FIFO is empty, otherwise false.
     */
    bool isTransmitEmpty();

    /**
     * @brief Resets the UART hardware.
     */
    void reset();

    /**
     * @brief Unresets the UART hardware.
     */
    void unreset();
};

#endif // AQISENSORSERIAL_HPP