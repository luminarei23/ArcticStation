/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

int main()
{
    stdio_init_all();

    while (true)
    {
        printf(">:( \n");
        sleep_ms(1000);
    }
}