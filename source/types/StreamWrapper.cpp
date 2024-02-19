#include "StreamWrapper.hpp"

#include "hardware/address_mapped.h"
#include "hardware/platform_defs.h"
#include "hardware/uart.h"

#include "hardware/structs/uart.h"
#include "hardware/resets.h"
#include "hardware/clocks.h"
#include "hardware/timer.h"

#include "pico/assert.h"
#include "pico.h"

Stream::Stream() : uart_(uart0) {}

void Stream::init(uint baudrate)
{
    if (clock_get_hz(clk_peri) == 0)
    {
        return;
    }
    reset();
    unreset();

    // Any LCR writes need to take place before enabling the UART
    uint baud = setBaudrate(baudrate);
    setFormat(8, 1, UART_PARITY_NONE);

    // Enable FIFOs (must be before setting UARTEN, as this is an LCR access)
    hw_set_bits(&uart_get_hw(uart_)->lcr_h, UART_UARTLCR_H_FEN_BITS);
    // Enable the UART, both TX and RX
    uart_get_hw(uart_)->cr = UART_UARTCR_UARTEN_BITS | UART_UARTCR_TXE_BITS | UART_UARTCR_RXE_BITS;
    // Always enable DREQ signals -- no harm in this if DMA is not listening
    uart_get_hw(uart_)->dmacr = UART_UARTDMACR_TXDMAE_BITS | UART_UARTDMACR_RXDMAE_BITS;
}

void Stream::write(const char *str)
{
    uart_puts(uart_, str);
}

bool Stream::isBusy()
{
    return !(uart_get_hw(uart_)->fr & UART_UARTFR_TXFF_BITS);
}

uint8_t Stream::read()
{
    while (!uart_is_readable(uart_))
        ; // Wait for character
    return uart_getc(uart_);
}

void Stream::flush()
{
    while (!isTransmitEmpty())
        ; // Wait for transmit FIFO to be empty
}

void Stream::reset()
{
    invalid_params_if(UART, uart_ != uart0 && uart_ != uart1);
    reset_block(uart_get_index(uart_) ? RESETS_RESET_UART1_BITS : RESETS_RESET_UART0_BITS);
}

void Stream::unreset()
{
    invalid_params_if(UART, uart_ != uart0 && uart_ != uart1);
    unreset_block_wait(uart_get_index(uart_) ? RESETS_RESET_UART1_BITS : RESETS_RESET_UART0_BITS);
}

uint32_t Stream::setBaudrate(uint baudrate)
{
    invalid_params_if(UART, baudrate == 0);
    uint32_t baud_rate_div = (8 * clock_get_hz(clk_peri) / baudrate);
    uint32_t baud_ibrd = baud_rate_div >> 7;
    uint32_t baud_fbrd;

    if (baud_ibrd == 0)
    {
        baud_ibrd = 1;
        baud_fbrd = 0;
    }
    else if (baud_ibrd >= 65535)
    {
        baud_ibrd = 65535;
        baud_fbrd = 0;
    }
    else
    {
        baud_fbrd = ((baud_rate_div & 0x7f) + 1) / 2;
    }

    uart_get_hw(uart_)->ibrd = baud_ibrd;
    uart_get_hw(uart_)->fbrd = baud_fbrd;

    // PL011 needs a (dummy) LCR_H write to latch in the divisors.
    // We don't want to actually change LCR_H contents here.
    uart_write_lcr_bits_masked(0, 0);

    // See datasheet
    return (4 * clock_get_hz(clk_peri)) / (64 * baud_ibrd + baud_fbrd);
}

void Stream::setFormat(uint data_bits, uint stop_bits, uart_parity_t parity)
{
    invalid_params_if(UART, data_bits < 5 || data_bits > 8);
    invalid_params_if(UART, stop_bits != 1 && stop_bits != 2);
    invalid_params_if(UART, parity != UART_PARITY_NONE && parity != UART_PARITY_EVEN && parity != UART_PARITY_ODD);

    uart_write_lcr_bits_masked(
        ((data_bits - 5u) << UART_UARTLCR_H_WLEN_LSB) |
            ((stop_bits - 1u) << UART_UARTLCR_H_STP2_LSB) |
            (bool_to_bit(parity != UART_PARITY_NONE) << UART_UARTLCR_H_PEN_LSB) |
            (bool_to_bit(parity == UART_PARITY_EVEN) << UART_UARTLCR_H_EPS_LSB),
        UART_UARTLCR_H_WLEN_BITS |
            UART_UARTLCR_H_STP2_BITS |
            UART_UARTLCR_H_PEN_BITS |
            UART_UARTLCR_H_EPS_BITS);
}

bool Stream::isTransmitEmpty()
{
    return uart_get_hw(uart_)->fr & UART_UARTFR_TXFE_BITS;
}

void Stream::uart_write_lcr_bits_masked(uint32_t values, uint32_t write_mask)
{
    // (Potentially) Cleanly handle disabling the UART before touching LCR
    uint32_t cr_save = uart_disable_before_lcr_write(uart_);

    hw_write_masked(&uart_get_hw(uart_)->lcr_h, values, write_mask);

    uart_get_hw(uart_)->cr = cr_save;
}

uint32_t Stream::uart_disable_before_lcr_write(uart_inst_t *uart)
{
    uint32_t cr_save = uart_get_hw(uart)->cr;

    if (cr_save & UART_UARTCR_UARTEN_BITS)
    {
        hw_clear_bits(&uart_get_hw(uart)->cr,
                      UART_UARTCR_UARTEN_BITS | UART_UARTCR_TXE_BITS | UART_UARTCR_RXE_BITS);

        uint32_t current_ibrd = uart_get_hw(uart)->ibrd;
        uint32_t current_fbrd = uart_get_hw(uart)->fbrd;

        // Note: Maximise precision here. Show working, the compiler will mop this up.
        // Create a 16.6 fixed-point fractional division ratio; then scale to 32-bits.
        uint32_t brdiv_ratio = 64u * current_ibrd + current_fbrd;
        brdiv_ratio <<= 10;
        // 3662 is ~(15 * 244.14) where 244.14 is 16e6 / 2^16
        uint32_t scaled_freq = clock_get_hz(clk_peri) / 3662ul;
        uint32_t wait_time_us = brdiv_ratio / scaled_freq;
        busy_wait_us(wait_time_us);
    }

    return cr_save;
}