#ifndef _STREAMWRAPPER_HPP_
#define _STREAMWRAPPER_HPP_

#include <cstdint>

#include "hardware/uart.h"
#include "pico.h"

#define BAUD_RATE 115200

/**
 * @brief A class representing a UART stream for communication.
 * The purpose is to have data read and written on uart 0 instance (PIN 0 and PIN 1)
 * of RPI Pico (for now uart0 is occupied by picoprobe /dev/ttyACM0)
 */
class Stream
{
public:
    /**
     * @brief Constructs a new Stream object with UART0 by default.
     */
    Stream();

    /**
     * @brief Initializes the UART stream with the specified baud rate.
     *
     * @param baudrate The baud rate for communication.
     */
    void init(uint baudrate);

    /**
     * @brief Writes a string to the UART stream.
     *
     * @param str The string to write.
     */
    void write(const char *str);

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
    uint8_t read();

    /**
     * @brief Flushes the transmit FIFO of the UART stream.
     */
    void flush();

    /**
     * @brief Resets the UART hardware.
     */
    void reset();

    /**
     * @brief Unresets the UART hardware.
     */
    void unreset();

    /**
     * @brief Sets the baud rate of the UART stream.
     *
     * @param baudrate The desired baud rate.
     * @return The actual baud rate set.
     */
    uint32_t setBaudrate(uint baudrate);

    /**
     * @brief Sets the format of the UART data.
     *
     * @param data_bits The number of data bits.
     * @param stop_bits The number of stop bits.
     * @param parity The parity mode.
     */
    void setFormat(uint data_bits, uint stop_bits, uart_parity_t parity);

    /**
     * @brief Checks if the UART transmit FIFO is empty.
     *
     * @return True if the transmit FIFO is empty, otherwise false.
     */
    bool isTransmitEmpty();

private:
    uart_inst_t *uart_; /**< The UART instance associated with this stream. */

    /**
     * @brief Disables the UART before writing to the LCR register.
     *
     * @param uart The UART instance to operate on.
     * @return The value of the control register before disabling.
     */
    uint32_t uart_disable_before_lcr_write(uart_inst_t *uart);

    /**
     * @brief Writes masked bits to the LCR register of the UART hardware.
     *
     * @param values The values to write.
     * @param write_mask The mask specifying which bits to write.
     */
    void uart_write_lcr_bits_masked(uint32_t values, uint32_t write_mask);
};
#endif